#include "dgus_bit_icon.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_bit_icon_t, dgus_bit_icon)
dgus_bit_icon_t *dgus_bit_icon_create(dgus_page_t *page,
                                      uint16_t id,
                                      uint16_t vp,
                                      uint16_t sp)
{
    dgus_bit_icon_t *icon;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    icon = (dgus_bit_icon_t *)dgus_page_alloc(page, sizeof(*icon));
    if (icon == NULL) {
        return NULL;
    }
    dgus_widget_init_obj(&icon->obj, id, DGUS_OBJ_BIT_ICON, vp, 3u);
    icon->sp = sp;
    if (dgus_page_add_obj(page, &icon->obj) != DGUS_OK) {
        return NULL;
    }

    return icon;
}

bool dgus_bit_icon_has_sp(const dgus_bit_icon_t *icon)
{
    return (icon != NULL) && dgus_widget_has_sp(icon->sp);
}

dgus_status_t dgus_bit_icon_set_bits(dgus_ui_t *ui,
                                     const dgus_bit_icon_t *icon,
                                     uint16_t bits)
{
    if (icon == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_set_u16(ui, &icon->obj, bits);
}

dgus_status_t dgus_bit_icon_desc_write_u16(dgus_ui_t *ui,
                                           const dgus_bit_icon_t *icon,
                                           uint16_t offset,
                                           uint16_t value)
{
    if (!dgus_bit_icon_has_sp(icon)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &icon->obj, icon->sp, offset, value);
}

dgus_status_t dgus_bit_icon_set_active_bits(dgus_ui_t *ui,
                                            const dgus_bit_icon_t *icon,
                                            uint16_t mask)
{
    return dgus_bit_icon_desc_write_u16(ui,
                                        icon,
                                        DGUS_BIT_ICON_SP_ACTIVE_BITS,
                                        mask);
}

dgus_status_t dgus_bit_icon_set_pos(dgus_ui_t *ui,
                                    const dgus_bit_icon_t *icon,
                                    int16_t x,
                                    int16_t y)
{
    dgus_point_t point;

    if (!dgus_bit_icon_has_sp(icon)) {
        return DGUS_ERR_ARG;
    }

    point.x = x;
    point.y = y;

    return dgus_obj_desc_write_point_at(ui, &icon->obj, icon->sp, DGUS_BIT_ICON_SP_POS, point);
}

dgus_status_t dgus_bit_icon_set_step(dgus_ui_t *ui,
                                     const dgus_bit_icon_t *icon,
                                     int16_t dx,
                                     int16_t dy)
{
    return dgus_bit_icon_desc_write_u16(ui,
                                        icon,
                                        DGUS_BIT_ICON_SP_STEP,
                                        dgus_pack_u8((uint8_t)dx,
                                                     (uint8_t)dy));
}

dgus_status_t dgus_bit_icon_set_icon_ids(dgus_ui_t *ui,
                                         const dgus_bit_icon_t *icon,
                                         uint16_t icon0_start,
                                         uint16_t icon0_end,
                                         uint16_t icon1_start,
                                         uint16_t icon1_end)
{
    uint16_t words[4];

    if (!dgus_bit_icon_has_sp(icon)) {
        return DGUS_ERR_ARG;
    }

    words[0] = icon0_start;
    words[1] = icon0_end;
    words[2] = icon1_start;
    words[3] = icon1_end;

    return dgus_obj_desc_write_words_at(ui, &icon->obj, icon->sp,
                                 DGUS_BIT_ICON_SP_ICON0_START,
                                 words,
                                 4u);
}
