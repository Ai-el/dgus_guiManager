#include "dgus_anim_icon.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_anim_icon_t, dgus_anim_icon)
dgus_anim_icon_t *dgus_anim_icon_create(dgus_page_t *page,
                                        uint16_t id,
                                        uint16_t vp,
                                        uint16_t sp)
{
    dgus_anim_icon_t *icon;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    icon = (dgus_anim_icon_t *)dgus_page_alloc(page, sizeof(dgus_anim_icon_t));
    if (icon == NULL) {
        return NULL;
    }
    dgus_widget_init_obj(&icon->obj, id, DGUS_OBJ_ANIM_ICON, vp, 2u);
    icon->sp = sp;
    if (dgus_page_add_obj(page, &icon->obj) != DGUS_OK) {
        return NULL;
    }

    return icon;
}

bool dgus_anim_icon_has_sp(const dgus_anim_icon_t *icon)
{
    return (icon != NULL) && dgus_widget_has_sp(icon->sp);
}

dgus_status_t dgus_anim_icon_set_value(dgus_ui_t *ui,
                                       const dgus_anim_icon_t *icon,
                                       uint16_t value)
{
    if (icon == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_set_u16(ui, &icon->obj, value);
}

dgus_status_t dgus_anim_icon_desc_write_u16(dgus_ui_t *ui,
                                            const dgus_anim_icon_t *icon,
                                            uint16_t offset,
                                            uint16_t value)
{
    if (!dgus_anim_icon_has_sp(icon)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &icon->obj, icon->sp, offset, value);
}

dgus_status_t dgus_anim_icon_desc_write_words(dgus_ui_t *ui,
                                              const dgus_anim_icon_t *icon,
                                              uint16_t offset,
                                              const uint16_t *words,
                                              uint8_t word_count)
{
    if (!dgus_anim_icon_has_sp(icon)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_words_at(ui, &icon->obj, icon->sp, offset, words, word_count);
}

dgus_status_t dgus_anim_icon_set_desc_vp(dgus_ui_t *ui,
                                         const dgus_anim_icon_t *icon,
                                         uint16_t vp)
{
    return dgus_anim_icon_desc_write_u16(ui, icon, DGUS_ANIM_ICON_SP_VP, vp);
}

dgus_status_t dgus_anim_icon_hide(dgus_ui_t *ui,
                                  const dgus_anim_icon_t *icon)
{
    return dgus_anim_icon_set_desc_vp(ui, icon, 0xFF00u);
}

dgus_status_t dgus_anim_icon_show(dgus_ui_t *ui,
                                  const dgus_anim_icon_t *icon)
{
    if (icon == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_anim_icon_set_desc_vp(ui, icon, icon->obj.vp);
}

dgus_status_t dgus_anim_icon_set_pos(dgus_ui_t *ui,
                                     const dgus_anim_icon_t *icon,
                                     int16_t x,
                                     int16_t y)
{
    dgus_point_t point;

    if (!dgus_anim_icon_has_sp(icon)) {
        return DGUS_ERR_ARG;
    }

    point.x = x;
    point.y = y;

    return dgus_obj_desc_write_point_at(ui, &icon->obj, icon->sp, DGUS_ANIM_ICON_SP_POS, point);
}

dgus_status_t dgus_anim_icon_set_values(dgus_ui_t *ui,
                                        const dgus_anim_icon_t *icon,
                                        uint16_t stop_value,
                                        uint16_t start_value)
{
    uint16_t words[2];

    words[0] = stop_value;
    words[1] = start_value;

    return dgus_anim_icon_desc_write_words(ui,
                                           icon,
                                           DGUS_ANIM_ICON_SP_V_STOP,
                                           words,
                                           2u);
}

dgus_status_t dgus_anim_icon_set_icon_range(dgus_ui_t *ui,
                                            const dgus_anim_icon_t *icon,
                                            uint16_t stop_icon,
                                            uint16_t start_icon,
                                            uint16_t end_icon)
{
    uint16_t words[3];

    words[0] = stop_icon;
    words[1] = start_icon;
    words[2] = end_icon;

    return dgus_anim_icon_desc_write_words(ui,
                                           icon,
                                           DGUS_ANIM_ICON_SP_ICON_STOP,
                                           words,
                                           3u);
}

dgus_status_t dgus_anim_icon_set_timing(dgus_ui_t *ui,
                                        const dgus_anim_icon_t *icon,
                                        uint8_t frame_time,
                                        dgus_anim_icon_play_mode_t play_mode)
{
    uint16_t words[2];

    words[0] = dgus_pack_u8(0u, frame_time);
    words[1] = dgus_pack_u8((uint8_t)play_mode, 0u);

    return dgus_anim_icon_desc_write_words(ui,
                                           icon,
                                           DGUS_ANIM_ICON_SP_GAMMA_TIME,
                                           words,
                                           2u);
}
