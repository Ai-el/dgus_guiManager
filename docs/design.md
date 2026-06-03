# DGUS HMI Design Notes

This document records the widget architecture and how it maps to the DGUS II
manual.

## Core Model

DGUS II screens are variable driven. The screen project owns pages, rendering,
touch hit testing, and most display behavior. The MCU communicates through
UART2 variable memory:

- `0x82`: write variable memory.
- `0x83`: read variable memory.
- Automatic touch/control uploads also use `0x83`.

The widget stack therefore has three layers:

- `dgus_t`: protocol frames, CRC, byte-stream RX parser, and common system variables.
- `dgus_ui_t`: pages, base objects, VP routing, and events.
- `dgus_runtime_t`: LVGL-like handler that pumps RX, ticks animations, polls system variables, and runs user VP watches.

The stack is platform-independent by design. It owns no UART peripheral, DMA
channel, RTOS object, GPIO, heap allocator, or interrupt registration. A BSP
connects the library by passing a `dgus_write_cb_t`, feeding received bytes, and
providing a millisecond tick if the built-in `dgus_tick_inc()` is not used.

## Base Object Boundary

`dgus_obj_t` is deliberately small:

```c
struct dgus_obj {
    uint16_t id;
    dgus_obj_type_t type;
    uint16_t vp;
    uint8_t word_count;
    uint16_t flags;
    dgus_ui_event_cb_t event_cb;
    void *user_data;
    dgus_widget_param_cb_t param_changed_cb;
    void *param_changed_user_data;
};
```

It has no SP address and no descriptor-operation table. The base object only supports:

- VP writes and reads.
- VP span matching for automatic uploads and poll responses.
- Event callback dispatch.
- Successful local VP/SP write notification for typed widget setters.
- Page ownership.

This keeps the base object honest: every DGUS display function has a different descriptor table, so a universal `obj_set_pos()` or `obj_hide()` would hide important manual differences.

Callbacks are layered in a predictable order:

```text
object callback
  -> page callback
  -> global UI callback
```

Use `dgus_obj_set_event_callback()` for a specific object, page callbacks for a
screen-level concern, and `dgus_ui_set_event_callback()` for diagnostics or
global routing.

Each typed widget also exposes two LVGL-style callback binders:

```c
dgus_button_set_trigger_callback(button, on_button_event, NULL);
dgus_var_icon_set_param_changed_callback(icon, on_icon_param, NULL);
```

`*_set_trigger_callback()` receives `dgus_ui_event_t`. It is for events that
come from the screen or from widget event promotion, such as touch/control
automatic uploads, `DGUS_UI_EVENT_VALUE_CHANGED`, touch-state events, and
`DGUS_UI_EVENT_CLICKED`.

`*_set_param_changed_callback()` receives `dgus_widget_param_event_t`. It is for
local setter success notifications. When application code calls a widget API
that writes VP data or a typed SP descriptor field, the callback reports:

- `space`: `DGUS_WIDGET_PARAM_VP` or `DGUS_WIDGET_PARAM_SP`.
- `address`: absolute DGUS word address written.
- `offset`: word offset from the widget's VP or SP base.
- `byte_len` and `bytes`: exact DGUS byte-order payload used for the write.
- `value`: first written word decoded as big-endian.

The callback fires only after the low-level write callback returns `DGUS_OK`.
It does not mean the screen has completed rendering; DGUS write ACK frames, when
enabled by the screen configuration, still arrive through `DGUS_UI_EVENT_WRITE_OK`.

## Widget Objects

Typed widgets embed `dgus_obj_t` as their first member, similar to LVGL widgets:

```c
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
} dgus_var_icon_t;
```

Create functions allocate from a caller-provided page pool and return typed pointers:

```c
dgus_var_icon_t *icon =
    dgus_var_icon_create(&page, 2, 0x1200, 0x8100);
```

VP data writes use the base object inside the widget:

```c
dgus_var_icon_set_value(&ui, icon, 3);
```

Descriptor writes use the widget API:

```c
dgus_var_icon_set_icon_range(&ui, icon, 10, 20);
dgus_var_icon_set_layer(&ui, icon, DGUS_LAYER_OVERWRITE, 0xFF);
```

Widget trigger and parameter callbacks are bound through the typed widget, not
through a BSP or global switch:

```c
static void icon_param_changed(const dgus_widget_param_event_t *event,
                               void *user_data)
{
    (void)user_data;
    if (event->space == DGUS_WIDGET_PARAM_SP) {
        /* Descriptor field changed, for example hide/show/position/color. */
    }
}

dgus_var_icon_set_param_changed_callback(icon, icon_param_changed, NULL);
```

If a widget has no descriptor pointer:

```c
dgus_text_t *text = dgus_text_create(&page, 3, 0x1300, DGUS_SP_NONE, 12);
```

then `dgus_text_has_sp(text)` is false and descriptor functions such as
`dgus_text_set_pos()` return `DGUS_ERR_ARG`. VP functions such as
`dgus_text_set_text_ascii()` still work.

## Why SP Is Widget Private

The manual's display variables reuse offsets for different meanings:

- Data variable `0x10`: `SP+1` is position, `SP+3` is color, `SP+4` is font, `SP+5..6` are digit/data-mode settings.
- Variable icon `0x00`: `SP+3..4` are value range, `SP+5..6` are icon range, `SP+7..8` are icon library/layer settings.
- Animated icon `0x01`: `SP+4..8` are stop/start values and icon range, `SP+0x0B..0x0C` are timing/playback fields.
- Video `0x31`: `SP+1..4` are position/window, `SP+5..7` are FPS/type/file IDs, while VP contains playback control and feedback.

For that reason, every widget has its own `.h/.c` pair and its own `DGUS_xxx_SP_*` constants. Application code should call widget methods such as `dgus_data_var_set_color()` or `dgus_video_set_window()` instead of writing magic offsets through the base object.

Widgets are grouped by the manual's chapter structure:

```text
widgets/display_variables/icon_display/variable_icon_display/
widgets/display_variables/text_variables/data_variable_display/
widgets/display_variables/graphic_variables/trend_curve_display/
widgets/display_variables/auxiliary_variables/digital_video_playback/
widgets/touch_controls/key_return/
```

The public header and implementation stay together inside each widget folder.

## Touch Abstraction

DGUS touch controls are not drawn or hit-tested by this library. They are defined in the DGUS project. When automatic upload is enabled, the screen sends:

```text
5A A5 LEN 83 VP_H VP_L WORDS DATA...
```

The receive path is:

```text
UART idle/DMA buffer
  -> dgus_feed()
  -> dgus_decode_var_frame()
  -> dgus_ui_find_obj_by_vp()
  -> VALUE_CHANGED event
  -> widget-specific event promotion, such as BUTTON CLICKED
```

This matches common embedded usage: an idle interrupt or DMA half/full callback captures bytes, then either directly calls `dgus_feed()` or places bytes in a ring buffer consumed by `dgus_timer_handler()`.

`0x0016` touch-state polling is separate. It produces `DGUS_UI_EVENT_TOUCH_PRESSED`,
`DGUS_UI_EVENT_TOUCH_PRESSING`, and `DGUS_UI_EVENT_TOUCH_RELEASED` events with
coordinates when the screen returns the touch-state system variable.

For clean interrupt boundaries, the runtime can own a caller-provided RX ring
buffer:

```c
static uint8_t rx_buf[256];

dgus_runtime_set_rx_buffer(&runtime, rx_buf, sizeof(rx_buf));

void uart_idle_callback(uint8_t *buf, size_t len)
{
    (void)dgus_runtime_rx_write(&runtime, buf, len);
}
```

The callback only copies bytes. `dgus_timer_handler()` later drains the ring
buffer, calls `dgus_feed()`, and dispatches UI events from the main loop.

## Runtime Handler

The runtime mirrors the useful shape of LVGL without copying LVGL's rendering model:

```c
dgus_tick_inc(ms);
dgus_tick_get();
dgus_timer_handler(&runtime);
```

The handler:

- Reads pending UART bytes through a board callback.
- Drains the optional runtime RX ring buffer.
- Feeds the protocol parser.
- Routes decoded frames into the UI event system.
- Advances fixed-pool animations.
- Optionally polls current page `0x0014`.
- Optionally polls touch state `0x0016`.
- Optionally polls application VP watches.

Poll requests are asynchronous. The eventual `0x83` response takes the same VP routing path as a touch automatic upload.

## Page Lifecycle

Pages can bind lightweight lifecycle callbacks:

```c
dgus_page_set_load_callback(&main_page, on_main_load, NULL);
dgus_page_set_unload_callback(&main_page, on_main_unload, NULL);
```

These callbacks run when the local active page changes, before the corresponding
page/global event callbacks. They are still platform-neutral and should only
call application code or enqueue work; they do not depend on any BSP.

## Animation Design

DGUS display movement is usually an SP write, but the SP offset differs per widget. The animation layer therefore does not call a base `obj_set_pos()`. A move animation stores a widget-specific position callback:

```c
typedef dgus_status_t (*dgus_anim_move_set_pos_cb_t)(dgus_ui_t *ui,
                                                     void *target,
                                                     int16_t x,
                                                     int16_t y);
```

Convenience initializers bind known widget setters:

```c
dgus_anim_data_var_move_init(...);
dgus_anim_text_move_init(...);
dgus_anim_var_icon_move_init(...);
```

Custom easing returns progress from `0` to `DGUS_ANIM_PROGRESS_MAX`, so project
code can implement its own ease-in/ease-out curve without changing the scheduler.

## Address Planning

Plan VP and SP ranges explicitly in the DGUS project and mirror them in code:

```c
#define VP_BTN_START   0x1001u
#define VP_TEMP_VALUE  0x1100u
#define SP_TEMP_VALUE  0x8000u
#define SP_ICON_STATE  0x8100u
```

Recommended practice:

- Keep touch return VPs and process-value VPs in a low, readable range.
- Keep descriptor SP blocks in a separate high range such as `0x8000+`.
- Allocate SP block sizes from the matching manual table, not from another widget.
- Use `DGUS_SP_NONE` when a widget should be data-only from the MCU side.

The library does not infer SP layouts from a project file. It assumes the VP/SP
addresses passed to create functions match the DGUS PC project.
