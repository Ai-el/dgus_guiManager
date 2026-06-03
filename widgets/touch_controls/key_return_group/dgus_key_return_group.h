#ifndef DGUS_KEY_RETURN_GROUP_H
#define DGUS_KEY_RETURN_GROUP_H

#include "dgus_obj.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dgus_key_return_group dgus_key_return_group_t;

typedef void (*dgus_key_return_group_cb_t)(const dgus_ui_event_t *event,
                                           void *user_data);

typedef struct {
    uint16_t key_value;
    dgus_key_return_group_cb_t cb;
    void *user_data;
} dgus_key_return_group_action_t;

struct dgus_key_return_group {
    dgus_obj_t obj;
    dgus_key_return_group_action_t *actions;
    uint16_t action_count;
    uint16_t action_capacity;
    dgus_key_return_group_cb_t fallback_cb;
    void *fallback_user_data;
};

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_key_return_group_t, dgus_key_return_group);

/* Create a key-return group object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS auto-upload return VP shared by multiple touch controls.
 * action_capacity: maximum number of value-to-callback bindings stored by the group.
 */
dgus_key_return_group_t *dgus_key_return_group_create(dgus_page_t *page,
                                                      uint16_t id,
                                                      uint16_t vp,
                                                      uint16_t action_capacity);

/* Bind one uploaded key value to a callback.
 * key_value: word uploaded by the DGUS key-return control.
 * cb: callback called when event->value equals key_value. The event code is
 *   DGUS_UI_EVENT_CLICKED when a key binding matches.
 * user_data: caller context passed back to cb.
 */
dgus_status_t dgus_key_return_group_bind(dgus_key_return_group_t *group,
                                         uint16_t key_value,
                                         dgus_key_return_group_cb_t cb,
                                         void *user_data);

/* Bind a fallback callback for unregistered key values.
 * cb: called after the object event callback when no key binding matches.
 * user_data: caller context passed back to cb.
 */
void dgus_key_return_group_set_fallback(dgus_key_return_group_t *group,
                                        dgus_key_return_group_cb_t cb,
                                        void *user_data);

/* Internal router helper used by dgus_ui_process_frame(). */
dgus_status_t dgus_key_return_group_handle_value_upload(dgus_obj_t *obj,
                                                        dgus_ui_event_t *event,
                                                        uint16_t value);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_KEY_RETURN_GROUP_H */
