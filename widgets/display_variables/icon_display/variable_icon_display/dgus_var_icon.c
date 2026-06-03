#include "dgus_var_icon.h"

#include "dgus_widget_common.h"

DGUS_WIDGET_DEFINE_CALLBACKS(dgus_var_icon_t, dgus_var_icon)
dgus_var_icon_t *dgus_var_icon_create(dgus_page_t *page,
                                      uint16_t id,
                                      uint16_t vp,
                                      uint16_t sp)
{
    dgus_var_icon_t *icon;

    if ((page == NULL) || (page->object_count >= page->object_capacity))
    {
        return NULL;
    }
    icon = (dgus_var_icon_t *)dgus_page_alloc(page, sizeof(dgus_var_icon_t));
    if (icon == NULL)
    {
        return NULL;
    }
    dgus_widget_init_obj(&icon->obj, id, DGUS_OBJ_VAR_ICON, vp, 1u);
    icon->sp = sp;
    if (dgus_page_add_obj(page, &icon->obj) != DGUS_OK)
    {
        return NULL;
    }

    return icon;
}

bool dgus_var_icon_has_sp(const dgus_var_icon_t *icon)
{
    return (icon != NULL) && dgus_widget_has_sp(icon->sp);
}

dgus_status_t dgus_var_icon_set_value(dgus_ui_t *ui,
                                      const dgus_var_icon_t *icon,
                                      uint16_t value)
{
    if (icon == NULL)
    {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_set_u16(ui, &icon->obj, value);
}

dgus_status_t dgus_var_icon_desc_write_u16(dgus_ui_t *ui,
                                           const dgus_var_icon_t *icon,
                                           uint16_t offset,
                                           uint16_t value)
{
    if (!dgus_var_icon_has_sp(icon))
    {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &icon->obj, icon->sp, offset, value);
}

dgus_status_t dgus_var_icon_desc_write_words(dgus_ui_t *ui,
                                             const dgus_var_icon_t *icon,
                                             uint16_t offset,
                                             const uint16_t *words,
                                             uint8_t word_count)
{
    if (!dgus_var_icon_has_sp(icon))
    {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_words_at(ui, &icon->obj, icon->sp, offset, words, word_count);
}

dgus_status_t dgus_var_icon_set_desc_vp(dgus_ui_t *ui,
                                        const dgus_var_icon_t *icon,
                                        uint16_t vp)
{
    return dgus_var_icon_desc_write_u16(ui, icon, DGUS_VAR_ICON_SP_VP, vp);
}

dgus_status_t dgus_var_icon_hide(dgus_ui_t *ui,
                                 const dgus_var_icon_t *icon)
{
    return dgus_var_icon_set_desc_vp(ui, icon, 0xFF00u);
}

dgus_status_t dgus_var_icon_show(dgus_ui_t *ui,
                                 const dgus_var_icon_t *icon)
{
    if (icon == NULL)
    {
        return DGUS_ERR_ARG;
    }

    return dgus_var_icon_set_desc_vp(ui, icon, icon->obj.vp);
}

dgus_status_t dgus_var_icon_set_pos(dgus_ui_t *ui,
                                    const dgus_var_icon_t *icon,
                                    int16_t x,
                                    int16_t y)
{
    dgus_point_t point;

    if (!dgus_var_icon_has_sp(icon))
    {
        return DGUS_ERR_ARG;
    }

    point.x = x;
    point.y = y;

    return dgus_obj_desc_write_point_at(ui, &icon->obj, icon->sp, DGUS_VAR_ICON_SP_POS, point);
}

dgus_status_t dgus_var_icon_set_value_range(dgus_ui_t *ui,
                                            const dgus_var_icon_t *icon,
                                            uint16_t min_value,
                                            uint16_t max_value)
{
    uint16_t words[2];

    words[0] = min_value;
    words[1] = max_value;

    return dgus_var_icon_desc_write_words(ui,
                                          icon,
                                          DGUS_VAR_ICON_SP_V_MIN,
                                          words,
                                          2u);
}

dgus_status_t dgus_var_icon_set_icon_range(dgus_ui_t *ui,
                                           const dgus_var_icon_t *icon,
                                           uint16_t min_icon,
                                           uint16_t max_icon)
{
    uint16_t words[2];

    words[0] = min_icon;
    words[1] = max_icon;

    return dgus_var_icon_desc_write_words(ui,
                                          icon,
                                          DGUS_VAR_ICON_SP_ICON_MIN,
                                          words,
                                          2u);
}

dgus_status_t dgus_var_icon_set_library_mode(dgus_ui_t *ui,
                                             const dgus_var_icon_t *icon,
                                             uint8_t icon_lib,
                                             dgus_icon_mode_t mode)
{
    return dgus_var_icon_desc_write_u16(ui,
                                        icon,
                                        DGUS_VAR_ICON_SP_ICON_LIB,
                                        dgus_pack_u8(icon_lib,
                                                     (uint8_t)mode));
}

dgus_status_t dgus_var_icon_set_layer(dgus_ui_t *ui,
                                      const dgus_var_icon_t *icon,
                                      dgus_layer_mode_t layer,
                                      uint8_t icon_gamma)
{
    return dgus_var_icon_desc_write_u16(ui,
                                        icon,
                                        DGUS_VAR_ICON_SP_LAYER,
                                        dgus_pack_u8((uint8_t)layer,
                                                     icon_gamma));
}
