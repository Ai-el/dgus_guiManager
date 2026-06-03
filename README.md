# DGUS HMI Widgets

C99 driver and LVGL-style UI manager for DWIN T5L/DGUS II UART2 screens.

The design follows the DGUS manual's variable model instead of treating the
screen as a framebuffer. The MCU writes VP data, optionally edits display
descriptor fields through SP, routes automatic uploads, and runs host-side
animations.

See `docs/design.md` for the detailed design notes.

## Portability Boundary

The library is intentionally BSP-independent. It does not include any MCU HAL,
RTOS, GPIO, DMA, or UART register code. A new main project or chip BSP only
needs to provide:

- a `dgus_write_cb_t` that writes bytes to the screen UART,
- optional calls to `dgus_tick_inc()` or a `dgus_tick_set_cb()` provider,
- optional calls to `dgus_runtime_rx_write()` from a UART idle/DMA callback.

All object storage, animation storage, VP watch tables, and RX buffers are
caller-owned static memory.

## Layout

- `include/dgus.h`: low-level UART2 protocol driver for `5A A5` frames, `0x82` writes, `0x83` reads/uploads, CRC, page switch, backlight, buzzer, and touch simulation.
- `include/dgus_obj.h`: base page/object/event model. The base object owns only VP data routing and generic VP read/write operations.
- `include/dgus_ui.h`: aggregate widget header.
- `include/dgus_anim.h`: fixed-pool animation manager with linear, ease-in, ease-out, ease-in-out, and custom easing callbacks.
- `include/dgus_runtime.h`: LVGL-like `dgus_tick_inc()`, `dgus_tick_get()`, and `dgus_timer_handler()` runtime layer.
- `widgets/display_variables/`: DGUS display-variable widgets grouped by manual category, with `.h` and `.c` kept together in each widget folder.
- `widgets/touch_controls/`: VP-backed touch helper widgets.
- `widgets/common/`: internal widget helper code.
- `examples/`: one runnable main example plus one create/use example for each small widget.

## VP/SP Model

DGUS uses two different address concepts:

- VP is the runtime data pointer. It stores values, text, touch returns, progress values, video control words, and similar data.
- SP is the display descriptor pointer. It stores display-function-specific fields such as linked VP, position, color, font, icon ranges, mode flags, and window rectangles.

`dgus_obj_t` intentionally has no SP field and no generic visual methods. It only provides data-oriented APIs:

```c
dgus_obj_set_u16(&ui, &obj->obj, 123);
dgus_obj_write_words(&ui, &obj->obj, words, 2);
dgus_obj_read(&ui, &obj->obj);
```

SP operations live on the matching widget type:

```c
dgus_data_var_t *temp = dgus_data_var_create(&page, 1, 0x1100, 0x8000);

dgus_data_var_set_u16(&ui, temp, 250);          /* VP 0x1100 */
dgus_data_var_set_pos(&ui, temp, 40, 60);       /* SP+1 for type 0x10 */
dgus_data_var_set_color(&ui, temp, 0xF800);     /* SP+3 for type 0x10 */
dgus_data_var_hide(&ui, temp);                  /* SP+0 = 0xFF00 */
```

If a widget is created with `DGUS_SP_NONE`, `dgus_xxx_has_sp()` returns false and its descriptor functions return `DGUS_ERR_ARG`. The object can still use VP data APIs.

## Widgets

Each manual display variable has its own header/source pair where its SP layout is defined:

- Icon/image: `widgets/display_variables/icon_display/...`
- Text/data: `widgets/display_variables/text_variables/...`
- Graphics: `widgets/display_variables/graphic_variables/...`
- Auxiliary/media: `widgets/display_variables/auxiliary_variables/...`
- Touch helpers: `widgets/touch_controls/...`

For example, variable icon display lives in:

```text
widgets/display_variables/icon_display/variable_icon_display/
    dgus_var_icon.h
    dgus_var_icon.c
```

This avoids a fake universal SP structure. For example, a variable icon's `SP+3..SP+8` fields mean value/icon ranges and library/layer settings, while a data variable's `SP+3..SP+6` fields mean color/font/digits/data mode.

## Touch Uploads

DGUS touch controls are configured in the PC project. When automatic upload is enabled, the screen sends a normal `0x83` variable frame:

```text
5A A5 06 83 VP_H VP_L 01 DATA_H DATA_L
```

The MCU can still receive frames using UART idle interrupt or DMA. Feed the captured bytes into the parser:

```c
void uart_idle_callback(uint8_t *buf, size_t len)
{
    dgus_feed(&dgus, buf, len);
}
```

For stricter interrupt boundaries, attach a runtime RX ring buffer and only push
bytes in the ISR/idle callback:

```c
static uint8_t dgus_rx_buf[256];

dgus_runtime_set_rx_buffer(&runtime, dgus_rx_buf, sizeof(dgus_rx_buf));

void uart_idle_callback(uint8_t *buf, size_t len)
{
    (void)dgus_runtime_rx_write(&runtime, buf, len);
}

while (1) {
    (void)dgus_timer_handler(&runtime);
}
```

The UI manager decodes `0x83`, finds the object whose VP span owns the address, emits `DGUS_UI_EVENT_VALUE_CHANGED`, and for `dgus_button_t` can emit `DGUS_UI_EVENT_CLICKED`.

Widget actions can be bound directly to the object:

```c
static void ok_clicked(void *user_data)
{
    (void)user_data;
}

dgus_button_t *ok = dgus_button_create(&page, 1, 0x1000);
dgus_button_set_key_value(ok, 0x0001);
dgus_button_set_clicked_callback(ok, ok_clicked, NULL);
```

Every widget also has typed callback binders:

```c
dgus_button_set_trigger_callback(ok, on_button_event, NULL);
dgus_text_set_param_changed_callback(title, on_title_param, NULL);
```

`*_set_trigger_callback()` is for screen-originated uploads and promoted widget
events. `*_set_param_changed_callback()` is for successful local widget setters,
including VP writes and typed SP descriptor changes such as position, color,
hide/show, ranges, and mode fields.

## Runtime

Use the runtime layer when you want an LVGL-like loop:

```c
void SysTick_Handler(void)
{
    dgus_tick_inc(1);
}

while (1) {
    uint32_t wait_ms = dgus_timer_handler(&runtime);
    board_sleep_ms(wait_ms);
}
```

The handler pumps RX bytes, advances animations, polls current page/touch state when enabled, and issues user VP watches. Reads are asynchronous; responses return through the same event path as automatic uploads.

Pages also support direct lifecycle callbacks through
`dgus_page_set_load_callback()` and `dgus_page_set_unload_callback()`, while
`dgus_obj_set_event_callback()` binds object-local event callbacks before page
and global UI callbacks.

## Animation

Animations are host-side schedulers. A move animation is not tied to the base object; it uses a widget-specific position setter:

```c
dgus_anim_move_t move;
dgus_anim_t anim;

dgus_anim_text_move_init(&move,
                         &anim,
                         &ui,
                         title,
                         20, 50,
                         220, 50,
                         800,
                         dgus_anim_path_ease_in_out);
dgus_anim_start(&anims, &anim);
```

Custom easing uses the same pattern as LVGL paths:

```c
static int32_t soft_start_stop(const dgus_anim_t *anim)
{
    int32_t p = dgus_anim_get_progress(anim);
    return (p * p * (3 * DGUS_ANIM_PROGRESS_MAX - 2 * p)) /
           (DGUS_ANIM_PROGRESS_MAX * DGUS_ANIM_PROGRESS_MAX);
}
```

Custom property animations can write any VP, widget SP field, or DGUS system variable from the executor callback.

## Build

Compile the runnable example with GCC:

```sh
gcc -std=c99 -Wall -Wextra -I dgus_hmi/include \
    -I dgus_hmi/widgets -I dgus_hmi/examples \
    dgus_hmi/src/dgus.c dgus_hmi/src/dgus_ui.c \
    $(find dgus_hmi/widgets -name '*.c') \
    dgus_hmi/src/dgus_anim.c dgus_hmi/src/dgus_runtime.c \
    $(find dgus_hmi/examples -name '*.c') -o dgus_examples
```

Or use CMake:

```cmake
add_subdirectory(dgus_hmi)
target_link_libraries(your_firmware PRIVATE dgus_hmi)
```

## Examples

- `examples/main.c`: primary basic-use example. It initializes the UART protocol driver, page/object pool, UI manager, runtime handler, VP watch polling, touch-upload routing, and a variable-icon move animation with custom ease-in-out path.
- `examples/example_widgets.h`: shared example context and all widget example prototypes.
- `examples/widgets/.../example_*.c`: one create/use function for each small widget, grouped with the same English manual-style categories as `widgets/`.

Each widget example creates one object from the page pool and demonstrates its normal VP writes plus its private descriptor/SP functions when the widget supports SP. The files are meant to be copied as usage references; only `examples/main.c` owns `main()`.
