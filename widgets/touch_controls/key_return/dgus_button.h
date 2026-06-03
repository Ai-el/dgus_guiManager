#ifndef DGUS_BUTTON_H
#define DGUS_BUTTON_H

#include "dgus_obj.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Key-return touch object. It owns only a VP return address.
 *
 * Parameters:
 * - vp is the touch-control return VP configured in the DGUS PC project.
 * - key_value is the uploaded word that should be treated as a click. Use 0 to
 *   accept any non-zero upload as clicked.
 * - auto_clear writes 0 back to vp after a click, matching common key-return
 *   button usage.
 * - event callbacks receive DGUS_UI_EVENT_VALUE_CHANGED for the raw upload and
 *   DGUS_UI_EVENT_CLICKED when the value matches key_value.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t key_value;
    dgus_ui_simple_cb_t clicked_cb;
    void *clicked_user_data;
} dgus_button_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_button_t, dgus_button);

/* Create a button object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS auto-upload return VP for this key control.
 */
dgus_button_t *dgus_button_create(dgus_page_t *page,
                                  uint16_t id,
                                  uint16_t vp);

/* Set the key value that makes a button emit DGUS_UI_EVENT_CLICKED.
 * key_value: expected uploaded word; 0 means any non-zero uploaded value.
 */
void dgus_button_set_key_value(dgus_button_t *button, uint16_t key_value);

/* Enable or disable automatic VP clearing after a button click.
 * enabled: true writes 0 to the button VP after CLICKED is emitted.
 */
void dgus_button_set_auto_clear(dgus_button_t *button, bool enabled);

/* Bind a button-local event callback.
 * cb: callback called for events routed to this button. Check event->code and
 *   handle DGUS_UI_EVENT_CLICKED for the button action.
 * user_data: caller context passed back to cb.
 * Passing NULL as cb removes the callback.
 */
void dgus_button_set_event_callback(dgus_button_t *button,
                                    dgus_ui_event_cb_t cb,
                                    void *user_data);

/* Bind a lightweight clicked callback.
 * cb: called only when the uploaded value matches key_value and the button
 *   emits DGUS_UI_EVENT_CLICKED.
 * user_data: caller context passed back to cb.
 * Passing NULL as cb removes the clicked callback.
 */
void dgus_button_set_clicked_callback(dgus_button_t *button,
                                      dgus_ui_simple_cb_t cb,
                                      void *user_data);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_BUTTON_H */
