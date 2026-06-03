#include "dgus_switch.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_switch_t, dgus_switch)
dgus_switch_t *dgus_switch_create(dgus_page_t *page,
                                  uint16_t id,
                                  uint16_t vp)
{
    dgus_switch_t *sw;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    sw = (dgus_switch_t *)dgus_page_alloc(page, sizeof(dgus_switch_t));
    if (sw == NULL) {
        return NULL;
    }
    dgus_widget_init_obj(&sw->obj, id, DGUS_OBJ_SWITCH, vp, 1u);
    sw->off_value = 0u;
    sw->on_value = 1u;
    sw->checked = false;
    if (dgus_page_add_obj(page, &sw->obj) != DGUS_OK) {
        return NULL;
    }

    return sw;
}

void dgus_switch_set_values(dgus_switch_t *sw,
                            uint16_t off_value,
                            uint16_t on_value)
{
    if (sw != NULL) {
        sw->off_value = off_value;
        sw->on_value = on_value;
    }
}

dgus_status_t dgus_switch_set_checked(dgus_ui_t *ui,
                                      dgus_switch_t *sw,
                                      bool checked)
{
    if (sw == NULL) {
        return DGUS_ERR_ARG;
    }

    sw->checked = checked;
    return dgus_obj_set_u16(ui,
                            &sw->obj,
                            checked ? sw->on_value : sw->off_value);
}
