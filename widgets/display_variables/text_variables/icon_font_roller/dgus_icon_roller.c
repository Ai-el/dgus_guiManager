#include "dgus_icon_roller.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_icon_roller_t, dgus_icon_roller)
dgus_icon_roller_t *dgus_icon_roller_create(dgus_page_t *page,
                                            uint16_t id,
                                            uint16_t vp,
                                            uint16_t sp)
{
    dgus_icon_roller_t *roller;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    roller = (dgus_icon_roller_t *)dgus_page_alloc(page, sizeof(*roller));
    if (roller == NULL) {
        return NULL;
    }
    dgus_widget_init_obj(&roller->obj, id, DGUS_OBJ_ICON_ROLLER, vp, 4u);
    roller->sp = sp;
    if (dgus_page_add_obj(page, &roller->obj) != DGUS_OK) {
        return NULL;
    }

    return roller;
}

bool dgus_icon_roller_has_sp(const dgus_icon_roller_t *roller)
{
    return (roller != NULL) && dgus_widget_has_sp(roller->sp);
}

dgus_status_t dgus_icon_roller_set_value(dgus_ui_t *ui,
                                         const dgus_icon_roller_t *roller,
                                         int16_t value)
{
    if (roller == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_set_i16(ui, &roller->obj, value);
}

dgus_status_t dgus_icon_roller_desc_write_u16(dgus_ui_t *ui,
                                              const dgus_icon_roller_t *roller,
                                              uint16_t offset,
                                              uint16_t value)
{
    if (!dgus_icon_roller_has_sp(roller)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &roller->obj, roller->sp, offset, value);
}

dgus_status_t dgus_icon_roller_set_desc_vp(dgus_ui_t *ui,
                                           const dgus_icon_roller_t *roller,
                                           uint16_t vp)
{
    return dgus_icon_roller_desc_write_u16(ui,
                                           roller,
                                           DGUS_ICON_ROLLER_SP_VP,
                                           vp);
}

dgus_status_t dgus_icon_roller_hide(dgus_ui_t *ui,
                                    const dgus_icon_roller_t *roller)
{
    return dgus_icon_roller_set_desc_vp(ui, roller, 0xFF00u);
}

dgus_status_t dgus_icon_roller_show(dgus_ui_t *ui,
                                    const dgus_icon_roller_t *roller)
{
    if (roller == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_icon_roller_set_desc_vp(ui, roller, roller->obj.vp);
}

dgus_status_t dgus_icon_roller_set_adjust_data_mode(dgus_ui_t *ui,
                                                    const dgus_icon_roller_t *roller,
                                                    uint8_t adjust_mode,
                                                    uint8_t data_mode)
{
    return dgus_icon_roller_desc_write_u16(ui,
                                           roller,
                                           DGUS_ICON_ROLLER_SP_ADJ_DATA,
                                           dgus_pack_u8(adjust_mode,
                                                        data_mode));
}

dgus_status_t dgus_icon_roller_set_step_or_string_vp(dgus_ui_t *ui,
                                                     const dgus_icon_roller_t *roller,
                                                     uint16_t value)
{
    return dgus_icon_roller_desc_write_u16(ui,
                                           roller,
                                           DGUS_ICON_ROLLER_SP_STRING_VP,
                                           value);
}

dgus_status_t dgus_icon_roller_set_range(dgus_ui_t *ui,
                                         const dgus_icon_roller_t *roller,
                                         int16_t min_value,
                                         int16_t max_value)
{
    uint16_t words[2];

    words[0] = (uint16_t)min_value;
    words[1] = (uint16_t)max_value;
    if (!dgus_icon_roller_has_sp(roller)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_words_at(ui, &roller->obj, roller->sp,
                                 DGUS_ICON_ROLLER_SP_V_MIN,
                                 words,
                                 2u);
}

dgus_status_t dgus_icon_roller_set_mode_speed(dgus_ui_t *ui,
                                              const dgus_icon_roller_t *roller,
                                              uint8_t display_mode,
                                              uint8_t speed)
{
    return dgus_icon_roller_desc_write_u16(ui,
                                           roller,
                                           DGUS_ICON_ROLLER_SP_MODE_SPEED,
                                           dgus_pack_u8(display_mode, speed));
}

dgus_status_t dgus_icon_roller_set_font_id(dgus_ui_t *ui,
                                           const dgus_icon_roller_t *roller,
                                           uint16_t font_id)
{
    return dgus_icon_roller_desc_write_u16(ui,
                                           roller,
                                           DGUS_ICON_ROLLER_SP_FONT_ID,
                                           font_id);
}

dgus_status_t dgus_icon_roller_set_selected_pos(dgus_ui_t *ui,
                                                const dgus_icon_roller_t *roller,
                                                int16_t x,
                                                int16_t y)
{
    dgus_point_t point;

    if (!dgus_icon_roller_has_sp(roller)) {
        return DGUS_ERR_ARG;
    }

    point.x = x;
    point.y = y;
    return dgus_obj_desc_write_point_at(ui, &roller->obj, roller->sp,
                                 DGUS_ICON_ROLLER_SP_SELECTED_POS,
                                 point);
}

dgus_status_t dgus_icon_roller_set_line_heights(dgus_ui_t *ui,
                                                const dgus_icon_roller_t *roller,
                                                uint8_t h0,
                                                uint8_t h1,
                                                uint8_t h2,
                                                uint8_t h3)
{
    uint16_t words[2];

    words[0] = dgus_pack_u8(h0, h1);
    words[1] = dgus_pack_u8(h2, h3);
    if (!dgus_icon_roller_has_sp(roller)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_words_at(ui, &roller->obj, roller->sp,
                                 DGUS_ICON_ROLLER_SP_LINE_HEIGHTS_0,
                                 words,
                                 2u);
}

dgus_status_t dgus_icon_roller_set_brightness_height(dgus_ui_t *ui,
                                                     const dgus_icon_roller_t *roller,
                                                     uint8_t dim_no_select,
                                                     uint8_t selected_height)
{
    return dgus_icon_roller_desc_write_u16(ui,
                                           roller,
                                           DGUS_ICON_ROLLER_SP_DIM_HEIGHT,
                                           dgus_pack_u8(dim_no_select,
                                                        selected_height));
}
