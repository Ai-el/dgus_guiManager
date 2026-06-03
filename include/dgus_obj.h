#ifndef DGUS_OBJ_H
#define DGUS_OBJ_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "dgus.h"
#include "dgus_widget_params.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DGUS_UI_OBJ_F_GLOBAL       0x0001u
#define DGUS_UI_OBJ_F_AUTO_CLEAR   0x0002u
#define DGUS_VP_NONE               0xFFFFu
#define DGUS_SP_NONE               0xFFFFu

typedef struct {
    int16_t x;
    int16_t y;
} dgus_point_t;

typedef struct {
    int16_t x0;
    int16_t y0;
    int16_t x1;
    int16_t y1;
} dgus_rect_t;

/* Logical object types used by the host-side UI manager. */
typedef enum {
    DGUS_OBJ_CUSTOM = 0,
    DGUS_OBJ_BUTTON,
    DGUS_OBJ_KEY_RETURN_GROUP,
    DGUS_OBJ_SWITCH,
    DGUS_OBJ_VAR_ICON,
    DGUS_OBJ_ANIM_ICON,
    DGUS_OBJ_SLIDER_INDICATOR,
    DGUS_OBJ_ART_TEXT,
    DGUS_OBJ_PIC_ANIM,
    DGUS_OBJ_ICON_ROTATE,
    DGUS_OBJ_BIT_ICON,
    DGUS_OBJ_JPEG_PAN,
    DGUS_OBJ_JPEG,
    DGUS_OBJ_BATCH_ICON,
    DGUS_OBJ_DATA_VAR,
    DGUS_OBJ_TEXT,
    DGUS_OBJ_RTC_TEXT,
    DGUS_OBJ_RTC_ANALOG,
    DGUS_OBJ_HEX,
    DGUS_OBJ_TEXT_SCROLL,
    DGUS_OBJ_DATA_WINDOW,
    DGUS_OBJ_DGUSII_TEXT,
    DGUS_OBJ_ICON_ROLLER,
    DGUS_OBJ_GTF_TEXT,
    DGUS_OBJ_TREND_CURVE,
    DGUS_OBJ_BASIC_GRAPH,
    DGUS_OBJ_PROGRESS,
    DGUS_OBJ_AREA_SCROLL,
    DGUS_OBJ_QR,
    DGUS_OBJ_AREA_BRIGHTNESS,
    DGUS_OBJ_DATA_TRANSFER,
    DGUS_OBJ_VIDEO,
    DGUS_OBJ_LABEL = DGUS_OBJ_TEXT,
    DGUS_OBJ_VALUE = DGUS_OBJ_DATA_VAR,
    DGUS_OBJ_BAR = DGUS_OBJ_PROGRESS
} dgus_obj_type_t;

/* Events emitted when DGUS frames are routed to pages and objects. */
typedef enum {
    DGUS_UI_EVENT_PAGE_LOAD = 0,
    DGUS_UI_EVENT_PAGE_UNLOAD,
    DGUS_UI_EVENT_VALUE_CHANGED,
    DGUS_UI_EVENT_CLICKED,
    DGUS_UI_EVENT_TOUCH_PRESSED,
    DGUS_UI_EVENT_TOUCH_PRESSING,
    DGUS_UI_EVENT_TOUCH_RELEASED,
    DGUS_UI_EVENT_WRITE_OK,
    DGUS_UI_EVENT_FRAME
} dgus_ui_event_code_t;

struct dgus_ui;
struct dgus_page;
struct dgus_obj;

typedef struct dgus_ui dgus_ui_t;
typedef struct dgus_page dgus_page_t;
typedef struct dgus_obj dgus_obj_t;

/* Address space changed by a widget setter.
 * DGUS_WIDGET_PARAM_VP means runtime data at the widget VP was written.
 * DGUS_WIDGET_PARAM_SP means a display descriptor field at the widget SP was
 * written by a typed widget API.
 */
typedef enum {
    DGUS_WIDGET_PARAM_VP = 0,
    DGUS_WIDGET_PARAM_SP
} dgus_widget_param_space_t;

/* Event payload passed to object, page, and global UI callbacks. */
typedef struct {
    dgus_ui_event_code_t code;
    dgus_ui_t *ui;
    dgus_page_t *page;
    dgus_obj_t *target;
    uint16_t vp;
    uint8_t word_count;
    const uint8_t *word_data;
    uint16_t value;
    uint8_t touch_state;
    uint16_t x;
    uint16_t y;
    const dgus_frame_t *frame;
} dgus_ui_event_t;

typedef void (*dgus_ui_event_cb_t)(const dgus_ui_event_t *event,
                                   void *user_data);

typedef void (*dgus_ui_simple_cb_t)(void *user_data);

/* Payload passed when a widget setter changes VP data or SP parameters.
 *
 * Parameters:
 * - ui is the UI manager used by the setter.
 * - target is the widget's embedded base object. Cast it to the known widget
 *   type when the callback is bound to a typed widget.
 * - space tells whether address is in VP data space or SP descriptor space.
 * - address is the absolute DGUS word address written on the screen.
 * - offset is the word offset from the widget's VP or SP base.
 * - byte_len is the number of bytes written.
 * - bytes points to the DGUS byte-order payload used for the write. It is only
 *   valid during the callback.
 * - value is the first written word decoded as big-endian, or 0 when byte_len
 *   is less than one DGUS word.
 */
typedef struct {
    dgus_ui_t *ui;
    const dgus_obj_t *target;
    dgus_widget_param_space_t space;
    uint16_t address;
    uint16_t offset;
    uint16_t byte_len;
    const uint8_t *bytes;
    uint16_t value;
} dgus_widget_param_event_t;

typedef void (*dgus_widget_param_cb_t)(const dgus_widget_param_event_t *event,
                                       void *user_data);

/* Declare typed callback binding functions for one widget type.
 *
 * Generated functions:
 * - prefix_set_trigger_callback(widget, cb, user_data)
 *   Binds the widget's screen-originated trigger callback. The callback
 *   receives dgus_ui_event_t and is called for routed VP uploads, touch-state
 *   events, and widget synthetic events such as DGUS_UI_EVENT_CLICKED.
 * - prefix_set_param_changed_callback(widget, cb, user_data)
 *   Binds the widget's local parameter callback. The callback receives
 *   dgus_widget_param_event_t and is called after a widget setter successfully
 *   writes VP data or a typed SP descriptor field.
 *
 * widget_type: concrete widget struct type, for example dgus_text_t.
 * prefix: function prefix without the trailing action name, for example
 *   dgus_text.
 */
#define DGUS_WIDGET_DECLARE_CALLBACKS(widget_type, prefix)                 \
    void prefix##_set_trigger_callback(widget_type *widget,                \
                                       dgus_ui_event_cb_t cb,              \
                                       void *user_data);                   \
    void prefix##_set_param_changed_callback(widget_type *widget,          \
                                             dgus_widget_param_cb_t cb,    \
                                             void *user_data)

/* Define typed callback binding functions for one widget type.
 * The widget struct must contain a dgus_obj_t member named obj.
 */
#define DGUS_WIDGET_DEFINE_CALLBACKS(widget_type, prefix)                  \
    void prefix##_set_trigger_callback(widget_type *widget,                \
                                       dgus_ui_event_cb_t cb,              \
                                       void *user_data)                    \
    {                                                                      \
        if (widget != NULL) {                                              \
            dgus_obj_set_trigger_callback(&widget->obj, cb, user_data);    \
        }                                                                  \
    }                                                                      \
    void prefix##_set_param_changed_callback(widget_type *widget,          \
                                             dgus_widget_param_cb_t cb,    \
                                             void *user_data)              \
    {                                                                      \
        if (widget != NULL) {                                              \
            dgus_obj_set_param_changed_callback(&widget->obj,              \
                                               cb,                         \
                                               user_data);                 \
        }                                                                  \
    }

/* Base object. It only owns VP data routing and generic VP read/write state.
 * DGUS display-variable descriptor pointers are private to each widget type.
 */
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

/* Host-side description of one DGUS page and its object table. */
struct dgus_page {
    uint16_t id;
    const char *name;
    dgus_obj_t **objects;
    uint16_t object_count;
    uint16_t object_capacity;
    uint8_t *memory;
    size_t memory_size;
    size_t memory_used;
    dgus_ui_event_cb_t event_cb;
    void *user_data;
    dgus_ui_simple_cb_t load_cb;
    void *load_user_data;
    dgus_ui_simple_cb_t unload_cb;
    void *unload_user_data;
};

/* UI manager context. Keep one instance for each physical DGUS display. */
struct dgus_ui {
    dgus_t *dev;
    dgus_page_t *pages;
    uint16_t page_count;
    dgus_page_t *active_page;
    dgus_ui_event_cb_t event_cb;
    void *user_data;
};

/* Initialize a user-owned base object. Prefer create functions for widgets. */
void dgus_obj_init(dgus_obj_t *obj,
                   uint16_t id,
                   dgus_obj_type_t type,
                   uint16_t vp,
                   uint8_t word_count,
                   dgus_ui_event_cb_t cb,
                   void *user_data);

/* Create a custom base object from the page pool. */
dgus_obj_t *dgus_obj_create(dgus_page_t *page,
                            uint16_t id,
                            uint16_t vp,
                            uint8_t word_count);

/* Set behavior flags such as DGUS_UI_OBJ_F_AUTO_CLEAR. */
void dgus_obj_set_flags(dgus_obj_t *obj, uint16_t flags);

/* Change the VP used for generic data writes and event routing. */
void dgus_obj_set_vp(dgus_obj_t *obj, uint16_t vp);

/* Set the number of DGUS words owned by the VP span. */
void dgus_obj_set_word_count(dgus_obj_t *obj, uint8_t word_count);

/* Bind an object-local event callback.
 * cb: callback called before page-level and UI-level callbacks.
 * user_data: caller context passed back to cb.
 * Passing NULL as cb removes the object callback.
 */
void dgus_obj_set_event_callback(dgus_obj_t *obj,
                                 dgus_ui_event_cb_t cb,
                                 void *user_data);

/* Bind an object-local trigger callback.
 * This is an LVGL-style alias for dgus_obj_set_event_callback().
 * cb: called for screen-originated events routed to this object, such as
 *   DGUS_UI_EVENT_VALUE_CHANGED, DGUS_UI_EVENT_TOUCH_PRESSED, and widget
 *   synthetic events such as DGUS_UI_EVENT_CLICKED.
 * user_data: caller context passed back to cb.
 * Passing NULL as cb removes the trigger callback.
 */
void dgus_obj_set_trigger_callback(dgus_obj_t *obj,
                                   dgus_ui_event_cb_t cb,
                                   void *user_data);

/* Bind a callback for successful local widget parameter changes.
 * cb: called after a widget setter writes VP data or an SP descriptor field
 *   successfully through the DGUS protocol driver.
 * user_data: caller context passed back to cb.
 * Passing NULL as cb removes the parameter callback.
 */
void dgus_obj_set_param_changed_callback(dgus_obj_t *obj,
                                         dgus_widget_param_cb_t cb,
                                         void *user_data);

/* Return an object's DGUS word span, defaulting empty spans to one word. */
uint8_t dgus_obj_get_word_count(const dgus_obj_t *obj);

/* Initialize a page descriptor with its object table. */
void dgus_page_init(dgus_page_t *page,
                    uint16_t id,
                    const char *name,
                    dgus_obj_t **objects,
                    uint16_t object_capacity,
                    void *memory,
                    size_t memory_size,
                    dgus_ui_event_cb_t cb,
                    void *user_data);

/* Add a user-owned object to a page. Create functions call this internally. */
dgus_status_t dgus_page_add_obj(dgus_page_t *page, dgus_obj_t *obj);

/* Bind a page-level event callback.
 * cb: callback called after the target object's callback and before the global
 *   UI callback.
 * user_data: caller context passed back to cb.
 * Passing NULL as cb removes the page callback.
 */
void dgus_page_set_event_callback(dgus_page_t *page,
                                  dgus_ui_event_cb_t cb,
                                  void *user_data);

/* Bind a page-load callback.
 * cb: called when the page becomes active locally.
 * user_data: caller context passed back to cb.
 */
void dgus_page_set_load_callback(dgus_page_t *page,
                                 dgus_ui_simple_cb_t cb,
                                 void *user_data);

/* Bind a page-unload callback.
 * cb: called when the page stops being active locally.
 * user_data: caller context passed back to cb.
 */
void dgus_page_set_unload_callback(dgus_page_t *page,
                                   dgus_ui_simple_cb_t cb,
                                   void *user_data);

/* Initialize the page/object UI manager. */
void dgus_ui_init(dgus_ui_t *ui,
                  dgus_t *dev,
                  dgus_page_t *pages,
                  uint16_t page_count);

/* Set a global event callback that receives all routed UI events. */
void dgus_ui_set_event_callback(dgus_ui_t *ui,
                                dgus_ui_event_cb_t cb,
                                void *user_data);

/* Find a page by its DGUS page ID. */
dgus_page_t *dgus_ui_find_page(dgus_ui_t *ui, uint16_t page_id);

/* Find an object by its host-side object ID inside one page. */
dgus_obj_t *dgus_page_find_obj(dgus_page_t *page, uint16_t obj_id);

/* Find the active/global object that owns a VP address. */
dgus_obj_t *dgus_ui_find_obj_by_vp(dgus_ui_t *ui, uint16_t vp);

/* Switch the DGUS screen to a page and update the local active page. */
dgus_status_t dgus_ui_set_page(dgus_ui_t *ui, uint16_t page_id);

/* Read 0x0014 so the host can synchronize the active DGUS page. */
dgus_status_t dgus_ui_request_active_page(dgus_ui_t *ui);

/* Route one decoded DGUS protocol frame into the UI event system. */
dgus_status_t dgus_ui_process_frame(dgus_ui_t *ui,
                                    const dgus_frame_t *frame);

/* Frame callback adapter for dgus_set_frame_callback(). */
void dgus_ui_frame_callback(dgus_t *dev,
                            const dgus_frame_t *frame,
                            void *user_data);

/* Write raw bytes to an object's VP address. */
dgus_status_t dgus_obj_write_bytes(dgus_ui_t *ui,
                                   const dgus_obj_t *obj,
                                   const uint8_t *data,
                                   uint16_t byte_len);

/* Write multiple DGUS words to an object's VP address. */
dgus_status_t dgus_obj_write_words(dgus_ui_t *ui,
                                   const dgus_obj_t *obj,
                                   const uint16_t *words,
                                   uint8_t word_count);

/* Write one unsigned 16-bit value to an object's VP address. */
dgus_status_t dgus_obj_set_u16(dgus_ui_t *ui,
                               const dgus_obj_t *obj,
                               uint16_t value);

/* Write one signed 16-bit value to an object's VP address. */
dgus_status_t dgus_obj_set_i16(dgus_ui_t *ui,
                               const dgus_obj_t *obj,
                               int16_t value);

/* Write one unsigned 32-bit value to an object's VP address. */
dgus_status_t dgus_obj_set_u32(dgus_ui_t *ui,
                               const dgus_obj_t *obj,
                               uint32_t value);

/* Read the words owned by an object. */
dgus_status_t dgus_obj_read(dgus_ui_t *ui, const dgus_obj_t *obj);

/* Write ASCII text as DGUS 16-bit text data and optionally append 0xFFFF. */
dgus_status_t dgus_obj_set_text_ascii(dgus_ui_t *ui,
                                      const dgus_obj_t *obj,
                                      const char *text,
                                      uint8_t max_chars,
                                      bool terminate);

/* Clear the DGUS touch-state update marker at 0x0016. */
dgus_status_t dgus_ui_clear_touch_state(dgus_ui_t *ui);



#ifdef __cplusplus
}
#endif

#endif /* DGUS_OBJ_H */
