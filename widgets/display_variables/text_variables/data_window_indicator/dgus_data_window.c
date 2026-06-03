#include "dgus_data_window.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_data_window_t, dgus_data_window)
dgus_data_window_t *dgus_data_window_create(dgus_page_t *page,
                                            uint16_t id,
                                            uint16_t vp,
                                            uint16_t sp)
{
    dgus_data_window_t *window;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    window = (dgus_data_window_t *)dgus_page_alloc(page, sizeof(*window));
    if (window == NULL) {
        return NULL;
    }
    dgus_widget_init_obj(&window->obj, id, DGUS_OBJ_DATA_WINDOW, vp, 2u);
    window->sp = sp;
    if (dgus_page_add_obj(page, &window->obj) != DGUS_OK) {
        return NULL;
    }

    return window;
}

bool dgus_data_window_has_sp(const dgus_data_window_t *window)
{
    return (window != NULL) && dgus_widget_has_sp(window->sp);
}

dgus_status_t dgus_data_window_set_value(dgus_ui_t *ui,
                                         const dgus_data_window_t *window,
                                         int16_t value)
{
    if (window == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_set_i16(ui, &window->obj, value);
}

dgus_status_t dgus_data_window_desc_write_u16(dgus_ui_t *ui,
                                              const dgus_data_window_t *window,
                                              uint16_t offset,
                                              uint16_t value)
{
    if (!dgus_data_window_has_sp(window)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &window->obj, window->sp, offset, value);
}

dgus_status_t dgus_data_window_set_desc_vp(dgus_ui_t *ui,
                                           const dgus_data_window_t *window,
                                           uint16_t vp)
{
    return dgus_data_window_desc_write_u16(ui,
                                           window,
                                           DGUS_DATA_WINDOW_SP_VP,
                                           vp);
}

dgus_status_t dgus_data_window_hide(dgus_ui_t *ui,
                                    const dgus_data_window_t *window)
{
    return dgus_data_window_set_desc_vp(ui, window, 0xFF00u);
}

dgus_status_t dgus_data_window_show(dgus_ui_t *ui,
                                    const dgus_data_window_t *window)
{
    if (window == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_data_window_set_desc_vp(ui, window, window->obj.vp);
}

dgus_status_t dgus_data_window_set_range(dgus_ui_t *ui,
                                         const dgus_data_window_t *window,
                                         int16_t min_value,
                                         int16_t max_value)
{
    uint16_t words[2];

    words[0] = (uint16_t)min_value;
    words[1] = (uint16_t)max_value;
    if (!dgus_data_window_has_sp(window)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_words_at(ui, &window->obj, window->sp,
                                 DGUS_DATA_WINDOW_SP_MIN,
                                 words,
                                 2u);
}

dgus_status_t dgus_data_window_set_digits(dgus_ui_t *ui,
                                          const dgus_data_window_t *window,
                                          uint8_t integer_digits,
                                          uint8_t decimal_digits)
{
    return dgus_data_window_desc_write_u16(ui,
                                           window,
                                           DGUS_DATA_WINDOW_SP_DIGITS,
                                           dgus_pack_u8(integer_digits,
                                                        decimal_digits));
}

dgus_status_t dgus_data_window_set_count_mode(dgus_ui_t *ui,
                                              const dgus_data_window_t *window,
                                              uint8_t count,
                                              uint8_t mode)
{
    return dgus_data_window_desc_write_u16(ui,
                                           window,
                                           DGUS_DATA_WINDOW_SP_COUNT_MODE,
                                           dgus_pack_u8(count, mode));
}

dgus_status_t dgus_data_window_set_center(dgus_ui_t *ui,
                                          const dgus_data_window_t *window,
                                          int16_t x,
                                          int16_t y)
{
    dgus_point_t point;

    if (!dgus_data_window_has_sp(window)) {
        return DGUS_ERR_ARG;
    }

    point.x = x;
    point.y = y;
    return dgus_obj_desc_write_point_at(ui, &window->obj, window->sp, DGUS_DATA_WINDOW_SP_CENTER, point);
}

dgus_status_t dgus_data_window_set_step(dgus_ui_t *ui,
                                        const dgus_data_window_t *window,
                                        uint16_t step)
{
    return dgus_data_window_desc_write_u16(ui,
                                           window,
                                           DGUS_DATA_WINDOW_SP_STEP,
                                           step);
}

dgus_status_t dgus_data_window_set_text_style(dgus_ui_t *ui,
                                              const dgus_data_window_t *window,
                                              uint8_t normal_x_dots,
                                              uint8_t normal_y_dots,
                                              uint16_t normal_color,
                                              uint8_t selected_x_dots,
                                              uint8_t selected_y_dots,
                                              uint16_t selected_color)
{
    uint16_t words[4];

    words[0] = dgus_pack_u8(normal_x_dots, normal_y_dots);
    words[1] = normal_color;
    words[2] = dgus_pack_u8(selected_x_dots, selected_y_dots);
    words[3] = selected_color;
    if (!dgus_data_window_has_sp(window)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_words_at(ui, &window->obj, window->sp,
                                 DGUS_DATA_WINDOW_SP_NORMAL_DOTS,
                                 words,
                                 4u);
}
