#include "dgus_area_scroll.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_area_scroll_t, dgus_area_scroll)
dgus_area_scroll_t *dgus_area_scroll_create(dgus_page_t *page,
                                            uint16_t id,
                                            uint16_t vp,
                                            uint16_t sp)
{
    dgus_area_scroll_t *scroll;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    scroll = (dgus_area_scroll_t *)dgus_page_alloc(page, sizeof(*scroll));
    if (scroll == NULL) {
        return NULL;
    }
    dgus_widget_init_obj(&scroll->obj, id, DGUS_OBJ_AREA_SCROLL, vp, 1u);
    scroll->sp = sp;
    if (dgus_page_add_obj(page, &scroll->obj) != DGUS_OK) {
        return NULL;
    }

    return scroll;
}

bool dgus_area_scroll_has_sp(const dgus_area_scroll_t *scroll)
{
    return (scroll != NULL) && dgus_widget_has_sp(scroll->sp);
}

dgus_status_t dgus_area_scroll_desc_write_u16(dgus_ui_t *ui,
                                              const dgus_area_scroll_t *scroll,
                                              uint16_t offset,
                                              uint16_t value)
{
    if (!dgus_area_scroll_has_sp(scroll)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &scroll->obj, scroll->sp, offset, value);
}

dgus_status_t dgus_area_scroll_set_area(dgus_ui_t *ui,
                                        const dgus_area_scroll_t *scroll,
                                        dgus_rect_t area)
{
    if (!dgus_area_scroll_has_sp(scroll)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_rect_at(ui, &scroll->obj, scroll->sp,
                                DGUS_AREA_SCROLL_SP_START,
                                area);
}

dgus_status_t dgus_area_scroll_set_speed(dgus_ui_t *ui,
                                         const dgus_area_scroll_t *scroll,
                                         uint16_t speed)
{
    return dgus_area_scroll_desc_write_u16(ui,
                                           scroll,
                                           DGUS_AREA_SCROLL_SP_SPEED,
                                           speed);
}

dgus_status_t dgus_area_scroll_set_direction(dgus_ui_t *ui,
                                             const dgus_area_scroll_t *scroll,
                                             dgus_scroll_dir_t dir)
{
    return dgus_area_scroll_desc_write_u16(ui,
                                           scroll,
                                           DGUS_AREA_SCROLL_SP_DIR,
                                           dgus_pack_u8((uint8_t)dir, 0u));
}
