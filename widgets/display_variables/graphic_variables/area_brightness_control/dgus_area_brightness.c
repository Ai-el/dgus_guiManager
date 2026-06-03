#include "dgus_area_brightness.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_area_brightness_t, dgus_area_brightness)
dgus_area_brightness_t *dgus_area_brightness_create(dgus_page_t *page,
                                                    uint16_t id,
                                                    uint16_t vp,
                                                    uint16_t sp)
{
    dgus_area_brightness_t *area;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    area = (dgus_area_brightness_t *)dgus_page_alloc(page, sizeof(*area));
    if (area == NULL) {
        return NULL;
    }
    dgus_widget_init_obj(&area->obj,
                         id,
                         DGUS_OBJ_AREA_BRIGHTNESS,
                         vp,
                         1u);
    area->sp = sp;
    if (dgus_page_add_obj(page, &area->obj) != DGUS_OK) {
        return NULL;
    }

    return area;
}

bool dgus_area_brightness_has_sp(const dgus_area_brightness_t *area)
{
    return (area != NULL) && dgus_widget_has_sp(area->sp);
}

dgus_status_t dgus_area_brightness_set_value(dgus_ui_t *ui,
                                             const dgus_area_brightness_t *area,
                                             uint16_t value_1_256)
{
    if (area == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_set_u16(ui, &area->obj, value_1_256);
}

dgus_status_t dgus_area_brightness_desc_write_u16(dgus_ui_t *ui,
                                                  const dgus_area_brightness_t *area,
                                                  uint16_t offset,
                                                  uint16_t value)
{
    if (!dgus_area_brightness_has_sp(area)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &area->obj, area->sp, offset, value);
}

dgus_status_t dgus_area_brightness_set_area(dgus_ui_t *ui,
                                            const dgus_area_brightness_t *area,
                                            dgus_rect_t rect)
{
    if (!dgus_area_brightness_has_sp(area)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_rect_at(ui, &area->obj, area->sp,
                                DGUS_AREA_BRIGHTNESS_SP_START,
                                rect);
}
