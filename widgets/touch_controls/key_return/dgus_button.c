#include "dgus_button.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_button_t, dgus_button)
dgus_button_t *dgus_button_create(dgus_page_t *page,
                                  uint16_t id,
                                  uint16_t vp)
{
    dgus_button_t *button;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    button = (dgus_button_t *)dgus_page_alloc(page, sizeof(dgus_button_t));
    if (button == NULL) {
        return NULL;
    }
    dgus_widget_init_obj(&button->obj, id, DGUS_OBJ_BUTTON, vp, 1u);
    button->key_value = 0u;
    button->clicked_cb = NULL;
    button->clicked_user_data = NULL;
    if (dgus_page_add_obj(page, &button->obj) != DGUS_OK) {
        return NULL;
    }

    return button;
}

void dgus_button_set_key_value(dgus_button_t *button, uint16_t key_value)
{
    if (button != NULL) {
        button->key_value = key_value;
    }
}

void dgus_button_set_auto_clear(dgus_button_t *button, bool enabled)
{
    if (button == NULL) {
        return;
    }

    if (enabled) {
        button->obj.flags |= DGUS_UI_OBJ_F_AUTO_CLEAR;
    } else {
        button->obj.flags &= (uint16_t)~DGUS_UI_OBJ_F_AUTO_CLEAR;
    }
}

void dgus_button_set_event_callback(dgus_button_t *button,
                                    dgus_ui_event_cb_t cb,
                                    void *user_data)
{
    if (button != NULL) {
        dgus_obj_set_event_callback(&button->obj, cb, user_data);
    }
}

void dgus_button_set_clicked_callback(dgus_button_t *button,
                                      dgus_ui_simple_cb_t cb,
                                      void *user_data)
{
    if (button != NULL) {
        button->clicked_cb = cb;
        button->clicked_user_data = user_data;
    }
}

dgus_status_t dgus_button_handle_value_upload(dgus_ui_t *ui,
                                              dgus_obj_t *obj,
                                              dgus_ui_event_t *event,
                                              uint16_t value)
{
    bool clicked;
    dgus_button_t *button;

    if ((obj == NULL) || (event == NULL)) {
        return DGUS_ERR_ARG;
    }

    button = (dgus_button_t *)obj;
    if (button->key_value == 0u) {
        clicked = (value != 0u);
    } else {
        clicked = (value == button->key_value);
    }

    if (!clicked) {
        return DGUS_OK;
    }

    event->code = DGUS_UI_EVENT_CLICKED;
    if (button->clicked_cb != NULL) {
        button->clicked_cb(button->clicked_user_data);
    }
    dgus_ui_dispatch_event(event);

    if (((obj->flags & DGUS_UI_OBJ_F_AUTO_CLEAR) != 0u) &&
        (ui != NULL) && (ui->dev != NULL)) {
        return dgus_obj_set_u16(ui, obj, 0u);
    }

    return DGUS_OK;
}
