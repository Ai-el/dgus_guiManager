#include "dgus_key_return_group.h"

#include "dgus_widget_common.h"

DGUS_WIDGET_DEFINE_CALLBACKS(dgus_key_return_group_t, dgus_key_return_group)

dgus_key_return_group_t *dgus_key_return_group_create(dgus_page_t *page,
                                                      uint16_t id,
                                                      uint16_t vp,
                                                      uint16_t action_capacity)
{
    dgus_key_return_group_t *group;

    if ((page == NULL) || (page->object_count >= page->object_capacity) ||
        (action_capacity == 0u)) {
        return NULL;
    }

    group = (dgus_key_return_group_t *)dgus_page_alloc(page, sizeof(*group));
    if (group == NULL) {
        return NULL;
    }

    group->actions = (dgus_key_return_group_action_t *)dgus_page_alloc(
        page,
        sizeof(group->actions[0]) * action_capacity);
    if (group->actions == NULL) {
        return NULL;
    }

    dgus_widget_init_obj(&group->obj,
                         id,
                         DGUS_OBJ_KEY_RETURN_GROUP,
                         vp,
                         1u);
    group->action_count = 0u;
    group->action_capacity = action_capacity;
    group->fallback_cb = NULL;
    group->fallback_user_data = NULL;

    if (dgus_page_add_obj(page, &group->obj) != DGUS_OK) {
        return NULL;
    }

    return group;
}

dgus_status_t dgus_key_return_group_bind(dgus_key_return_group_t *group,
                                         uint16_t key_value,
                                         dgus_key_return_group_cb_t cb,
                                         void *user_data)
{
    uint16_t i;

    if ((group == NULL) || (cb == NULL)) {
        return DGUS_ERR_ARG;
    }

    for (i = 0u; i < group->action_count; i++) {
        if (group->actions[i].key_value == key_value) {
            group->actions[i].cb = cb;
            group->actions[i].user_data = user_data;
            return DGUS_OK;
        }
    }

    if (group->action_count >= group->action_capacity) {
        return DGUS_ERR_OVERFLOW;
    }

    group->actions[group->action_count].key_value = key_value;
    group->actions[group->action_count].cb = cb;
    group->actions[group->action_count].user_data = user_data;
    group->action_count++;

    return DGUS_OK;
}

void dgus_key_return_group_set_fallback(dgus_key_return_group_t *group,
                                        dgus_key_return_group_cb_t cb,
                                        void *user_data)
{
    if (group != NULL) {
        group->fallback_cb = cb;
        group->fallback_user_data = user_data;
    }
}

dgus_status_t dgus_key_return_group_handle_value_upload(dgus_obj_t *obj,
                                                        dgus_ui_event_t *event,
                                                        uint16_t value)
{
    dgus_key_return_group_t *group;
    uint16_t i;

    if ((obj == NULL) || (event == NULL)) {
        return DGUS_ERR_ARG;
    }

    group = (dgus_key_return_group_t *)obj;
    for (i = 0u; i < group->action_count; i++) {
        if (group->actions[i].key_value == value) {
            event->code = DGUS_UI_EVENT_CLICKED;
            dgus_ui_dispatch_event(event);
            group->actions[i].cb(event, group->actions[i].user_data);
            return DGUS_OK;
        }
    }

    if (group->fallback_cb != NULL) {
        group->fallback_cb(event, group->fallback_user_data);
    }

    return DGUS_OK;
}
