#include "dgus_data_var.h"

#include "dgus_widget_common.h"

DGUS_WIDGET_DEFINE_CALLBACKS(dgus_data_var_t, dgus_data_var)
DGUS_WIDGET_DEFINE_CALLBACKS(dgus_value_t, dgus_value)
dgus_data_var_t *dgus_data_var_create(dgus_page_t *page,
                                      uint16_t id,
                                      uint16_t vp,
                                      uint16_t sp)
{
    dgus_data_var_t *data;

    if ((page == NULL) || (page->object_count >= page->object_capacity))
    {
        return NULL;
    }
    data = (dgus_data_var_t *)dgus_page_alloc(page, sizeof(dgus_data_var_t));
    if (data == NULL)
    {
        return NULL;
    }
    dgus_widget_init_obj(&data->obj, id, DGUS_OBJ_DATA_VAR, vp, 1u);
    data->sp = sp;
    data->min_value = 0;
    data->max_value = 0xFFFF;
    data->decimals = 0u;
    if (dgus_page_add_obj(page, &data->obj) != DGUS_OK)
    {
        return NULL;
    }

    return data;
}

dgus_value_t *dgus_value_create(dgus_page_t *page,
                                uint16_t id,
                                uint16_t vp,
                                uint16_t sp)
{
    return dgus_data_var_create(page, id, vp, sp);
}

bool dgus_data_var_has_sp(const dgus_data_var_t *data)
{
    return (data != NULL) && dgus_widget_has_sp(data->sp);
}

void dgus_data_var_set_range(dgus_data_var_t *data,
                             int32_t min_value,
                             int32_t max_value)
{
    if (data != NULL)
    {
        data->min_value = min_value;
        data->max_value = max_value;
    }
}

void dgus_value_set_range(dgus_value_t *value,
                          int32_t min_value,
                          int32_t max_value)
{
    dgus_data_var_set_range(value, min_value, max_value);
}

dgus_status_t dgus_data_var_set_u16(dgus_ui_t *ui,
                                    const dgus_data_var_t *data,
                                    uint16_t value)
{
    if (data == NULL)
    {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_set_u16(ui, &data->obj, value);
}

dgus_status_t dgus_value_set_u16(dgus_ui_t *ui,
                                 const dgus_value_t *value,
                                 uint16_t data)
{
    return dgus_data_var_set_u16(ui, value, data);
}

dgus_status_t dgus_data_var_desc_write_u16(dgus_ui_t *ui,
                                           const dgus_data_var_t *data,
                                           uint16_t offset,
                                           uint16_t value)
{
    if (!dgus_data_var_has_sp(data))
    {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &data->obj, data->sp, offset, value);
}

dgus_status_t dgus_data_var_desc_write_words(dgus_ui_t *ui,
                                             const dgus_data_var_t *data,
                                             uint16_t offset,
                                             const uint16_t *words,
                                             uint8_t word_count)
{
    if (!dgus_data_var_has_sp(data))
    {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_words_at(ui, &data->obj, data->sp, offset, words, word_count);
}

dgus_status_t dgus_data_var_set_desc_vp(dgus_ui_t *ui,
                                        const dgus_data_var_t *data,
                                        uint16_t vp)
{
    return dgus_data_var_desc_write_u16(ui, data, DGUS_DATA_VAR_SP_VP, vp);
}

dgus_status_t dgus_data_var_hide(dgus_ui_t *ui,
                                 const dgus_data_var_t *data)
{
    return dgus_data_var_set_desc_vp(ui, data, 0xFF00u);
}

dgus_status_t dgus_data_var_show(dgus_ui_t *ui,
                                 const dgus_data_var_t *data)
{
    if (data == NULL)
    {
        return DGUS_ERR_ARG;
    }

    return dgus_data_var_set_desc_vp(ui, data, data->obj.vp);
}

dgus_status_t dgus_data_var_set_pos(dgus_ui_t *ui,
                                    const dgus_data_var_t *data,
                                    int16_t x,
                                    int16_t y)
{
    dgus_point_t point;

    if (!dgus_data_var_has_sp(data))
    {
        return DGUS_ERR_ARG;
    }

    point.x = x;
    point.y = y;

    return dgus_obj_desc_write_point_at(ui, &data->obj, data->sp, DGUS_DATA_VAR_SP_POS, point);
}

dgus_status_t dgus_data_var_set_color(dgus_ui_t *ui,
                                      const dgus_data_var_t *data,
                                      uint16_t rgb565)
{
    return dgus_data_var_desc_write_u16(ui,
                                        data,
                                        DGUS_DATA_VAR_SP_COLOR,
                                        rgb565);
}

dgus_status_t dgus_data_var_set_font(dgus_ui_t *ui,
                                     const dgus_data_var_t *data,
                                     uint8_t font_id,
                                     uint8_t dot_size)
{
    return dgus_data_var_desc_write_u16(ui,
                                        data,
                                        DGUS_DATA_VAR_SP_FONT,
                                        dgus_pack_u8(font_id, dot_size));
}

dgus_status_t dgus_data_var_set_integer_digits(dgus_ui_t *ui,
                                               const dgus_data_var_t *data,
                                               dgus_align_t align,
                                               uint8_t digits)
{
    return dgus_data_var_desc_write_u16(ui,
                                        data,
                                        DGUS_DATA_VAR_SP_INT_DIGITS,
                                        dgus_pack_u8((uint8_t)align, digits));
}

dgus_status_t dgus_data_var_set_decimal_mode(dgus_ui_t *ui,
                                             const dgus_data_var_t *data,
                                             uint8_t decimal_digits,
                                             dgus_data_mode_t mode)
{
    return dgus_data_var_desc_write_u16(ui,
                                        data,
                                        DGUS_DATA_VAR_SP_DEC_MODE,
                                        dgus_pack_u8(decimal_digits,
                                                     (uint8_t)mode));
}
