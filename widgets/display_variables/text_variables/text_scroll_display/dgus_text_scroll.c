#include "dgus_text_scroll.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_text_scroll_t, dgus_text_scroll)
dgus_text_scroll_t *dgus_text_scroll_create(dgus_page_t *page,
                                            uint16_t id,
                                            uint16_t vp,
                                            uint16_t sp,
                                            uint8_t max_chars)
{
    dgus_text_scroll_t *scroll;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    scroll = (dgus_text_scroll_t *)dgus_page_alloc(page, sizeof(*scroll));
    if (scroll == NULL) {
        return NULL;
    }
    dgus_widget_init_obj(&scroll->obj,
                         id,
                         DGUS_OBJ_TEXT_SCROLL,
                         vp,
                         (uint8_t)(max_chars + DGUS_TEXT_SCROLL_VP_TEXT_OFFSET + 1u));
    scroll->sp = sp;
    scroll->max_chars = max_chars;
    scroll->terminate = true;
    if (dgus_page_add_obj(page, &scroll->obj) != DGUS_OK) {
        return NULL;
    }

    return scroll;
}

bool dgus_text_scroll_has_sp(const dgus_text_scroll_t *scroll)
{
    return (scroll != NULL) && dgus_widget_has_sp(scroll->sp);
}

void dgus_text_scroll_set_terminate(dgus_text_scroll_t *scroll, bool terminate)
{
    if (scroll != NULL) {
        scroll->terminate = terminate;
    }
}

dgus_status_t dgus_text_scroll_set_text_ascii(dgus_ui_t *ui,
                                              const dgus_text_scroll_t *scroll,
                                              const char *text)
{
    uint8_t data[DGUS_TX_BUFFER_SIZE];
    uint16_t data_len = 0u;
    uint8_t i;

    if ((scroll == NULL) || (text == NULL)) {
        return DGUS_ERR_ARG;
    }

    for (i = 0u; (i < scroll->max_chars) && (text[i] != '\0'); i++) {
        if ((uint16_t)(data_len + 2u) > (uint16_t)sizeof(data)) {
            return DGUS_ERR_OVERFLOW;
        }
        data[data_len] = 0x00u;
        data[data_len + 1u] = (uint8_t)text[i];
        data_len = (uint16_t)(data_len + 2u);
    }

    if (scroll->terminate) {
        if ((uint16_t)(data_len + 2u) > (uint16_t)sizeof(data)) {
            return DGUS_ERR_OVERFLOW;
        }
        data[data_len] = 0xFFu;
        data[data_len + 1u] = 0xFFu;
        data_len = (uint16_t)(data_len + 2u);
    }

    if (data_len == 0u) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_write_bytes_at(ui,
                                   &scroll->obj,
                                   DGUS_TEXT_SCROLL_VP_TEXT_OFFSET,
                                   data,
                                   data_len);
}

dgus_status_t dgus_text_scroll_desc_write_u16(dgus_ui_t *ui,
                                              const dgus_text_scroll_t *scroll,
                                              uint16_t offset,
                                              uint16_t value)
{
    if (!dgus_text_scroll_has_sp(scroll)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &scroll->obj, scroll->sp, offset, value);
}

dgus_status_t dgus_text_scroll_set_desc_vp(dgus_ui_t *ui,
                                           const dgus_text_scroll_t *scroll,
                                           uint16_t vp)
{
    return dgus_text_scroll_desc_write_u16(ui,
                                           scroll,
                                           DGUS_TEXT_SCROLL_SP_VP,
                                           vp);
}

dgus_status_t dgus_text_scroll_hide(dgus_ui_t *ui,
                                    const dgus_text_scroll_t *scroll)
{
    return dgus_text_scroll_set_desc_vp(ui, scroll, 0xFF00u);
}

dgus_status_t dgus_text_scroll_show(dgus_ui_t *ui,
                                    const dgus_text_scroll_t *scroll)
{
    if (scroll == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_text_scroll_set_desc_vp(ui, scroll, scroll->obj.vp);
}

dgus_status_t dgus_text_scroll_set_rolling(dgus_ui_t *ui,
                                           const dgus_text_scroll_t *scroll,
                                           dgus_text_scroll_mode_t mode,
                                           uint8_t distance)
{
    return dgus_text_scroll_desc_write_u16(ui,
                                           scroll,
                                           DGUS_TEXT_SCROLL_SP_ROLLING,
                                           dgus_pack_u8((uint8_t)mode, distance));
}

dgus_status_t dgus_text_scroll_set_align(dgus_ui_t *ui,
                                         const dgus_text_scroll_t *scroll,
                                         dgus_text_scroll_align_t align)
{
    return dgus_text_scroll_desc_write_u16(ui,
                                           scroll,
                                           DGUS_TEXT_SCROLL_SP_ADJUST,
                                           dgus_pack_u8((uint8_t)align, 0u));
}

dgus_status_t dgus_text_scroll_set_color(dgus_ui_t *ui,
                                         const dgus_text_scroll_t *scroll,
                                         uint16_t rgb565)
{
    return dgus_text_scroll_desc_write_u16(ui,
                                           scroll,
                                           DGUS_TEXT_SCROLL_SP_COLOR,
                                           rgb565);
}

dgus_status_t dgus_text_scroll_set_box(dgus_ui_t *ui,
                                       const dgus_text_scroll_t *scroll,
                                       dgus_rect_t box)
{
    if (!dgus_text_scroll_has_sp(scroll)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_rect_at(ui, &scroll->obj, scroll->sp, DGUS_TEXT_SCROLL_SP_BOX, box);
}

dgus_status_t dgus_text_scroll_set_font_ids(dgus_ui_t *ui,
                                            const dgus_text_scroll_t *scroll,
                                            uint8_t ascii_font,
                                            uint8_t non_ascii_font)
{
    return dgus_text_scroll_desc_write_u16(ui,
                                           scroll,
                                           DGUS_TEXT_SCROLL_SP_FONT_ID,
                                           dgus_pack_u8(ascii_font,
                                                        non_ascii_font));
}

dgus_status_t dgus_text_scroll_set_font_dots(dgus_ui_t *ui,
                                             const dgus_text_scroll_t *scroll,
                                             uint8_t x_dots,
                                             uint8_t y_dots)
{
    return dgus_text_scroll_desc_write_u16(ui,
                                           scroll,
                                           DGUS_TEXT_SCROLL_SP_FONT_DOTS,
                                           dgus_pack_u8(x_dots, y_dots));
}

dgus_status_t dgus_text_scroll_set_encode_spacing(dgus_ui_t *ui,
                                                  const dgus_text_scroll_t *scroll,
                                                  uint8_t encode_mode,
                                                  uint8_t horizontal_spacing)
{
    return dgus_text_scroll_desc_write_u16(ui,
                                           scroll,
                                           DGUS_TEXT_SCROLL_SP_ENCODE_SPACING,
                                           dgus_pack_u8(encode_mode,
                                                        horizontal_spacing));
}
