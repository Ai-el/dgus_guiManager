#include "dgus_dgusii_text.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_dgusii_text_t, dgus_dgusii_text)
dgus_dgusii_text_t *dgus_dgusii_text_create(dgus_page_t *page,
                                            uint16_t id,
                                            uint16_t vp,
                                            uint16_t sp,
                                            uint16_t max_bytes)
{
    dgus_dgusii_text_t *text;
    uint16_t word_count = (uint16_t)((max_bytes + 1u) / 2u);

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    if (word_count == 0u) {
        word_count = 1u;
    }
    if (word_count > 255u) {
        word_count = 255u;
    }
    text = (dgus_dgusii_text_t *)dgus_page_alloc(page, sizeof(*text));
    if (text == NULL) {
        return NULL;
    }
    dgus_widget_init_obj(&text->obj,
                         id,
                         DGUS_OBJ_DGUSII_TEXT,
                         vp,
                         (uint8_t)word_count);
    text->sp = sp;
    text->max_bytes = max_bytes;
    text->terminate = true;
    if (dgus_page_add_obj(page, &text->obj) != DGUS_OK) {
        return NULL;
    }

    return text;
}

bool dgus_dgusii_text_has_sp(const dgus_dgusii_text_t *text)
{
    return (text != NULL) && dgus_widget_has_sp(text->sp);
}

void dgus_dgusii_text_set_terminate(dgus_dgusii_text_t *text, bool terminate)
{
    if (text != NULL) {
        text->terminate = terminate;
    }
}

dgus_status_t dgus_dgusii_text_write_bytes(dgus_ui_t *ui,
                                           const dgus_dgusii_text_t *text,
                                           const uint8_t *bytes,
                                           uint16_t byte_len)
{
    if ((text == NULL) || (bytes == NULL) || (byte_len == 0u)) {
        return DGUS_ERR_ARG;
    }
    if ((text->max_bytes != 0u) && (byte_len > text->max_bytes)) {
        return DGUS_ERR_OVERFLOW;
    }

    return dgus_obj_write_bytes(ui, &text->obj, bytes, byte_len);
}

dgus_status_t dgus_dgusii_text_set_text_ascii(dgus_ui_t *ui,
                                              const dgus_dgusii_text_t *text,
                                              const char *value)
{
    uint8_t max_chars;

    if (text == NULL) {
        return DGUS_ERR_ARG;
    }
    max_chars = (text->max_bytes > 510u) ? 255u : (uint8_t)(text->max_bytes / 2u);

    return dgus_obj_set_text_ascii(ui,
                                   &text->obj,
                                   value,
                                   max_chars,
                                   text->terminate);
}

dgus_status_t dgus_dgusii_text_desc_write_u16(dgus_ui_t *ui,
                                              const dgus_dgusii_text_t *text,
                                              uint16_t offset,
                                              uint16_t value)
{
    if (!dgus_dgusii_text_has_sp(text)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &text->obj, text->sp, offset, value);
}

dgus_status_t dgus_dgusii_text_set_desc_vp(dgus_ui_t *ui,
                                           const dgus_dgusii_text_t *text,
                                           uint16_t vp)
{
    return dgus_dgusii_text_desc_write_u16(ui,
                                           text,
                                           DGUS_DGUSII_TEXT_SP_VP,
                                           vp);
}

dgus_status_t dgus_dgusii_text_hide(dgus_ui_t *ui,
                                    const dgus_dgusii_text_t *text)
{
    return dgus_dgusii_text_set_desc_vp(ui, text, 0xFF00u);
}

dgus_status_t dgus_dgusii_text_show(dgus_ui_t *ui,
                                    const dgus_dgusii_text_t *text)
{
    if (text == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_dgusii_text_set_desc_vp(ui, text, text->obj.vp);
}

dgus_status_t dgus_dgusii_text_set_pos(dgus_ui_t *ui,
                                       const dgus_dgusii_text_t *text,
                                       int16_t x,
                                       int16_t y)
{
    dgus_point_t point;

    if (!dgus_dgusii_text_has_sp(text)) {
        return DGUS_ERR_ARG;
    }

    point.x = x;
    point.y = y;
    return dgus_obj_desc_write_point_at(ui, &text->obj, text->sp, DGUS_DGUSII_TEXT_SP_POS, point);
}

dgus_status_t dgus_dgusii_text_set_color(dgus_ui_t *ui,
                                         const dgus_dgusii_text_t *text,
                                         uint16_t rgb565)
{
    return dgus_dgusii_text_desc_write_u16(ui,
                                           text,
                                           DGUS_DGUSII_TEXT_SP_COLOR,
                                           rgb565);
}

dgus_status_t dgus_dgusii_text_set_box(dgus_ui_t *ui,
                                       const dgus_dgusii_text_t *text,
                                       dgus_rect_t box)
{
    if (!dgus_dgusii_text_has_sp(text)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_rect_at(ui, &text->obj, text->sp, DGUS_DGUSII_TEXT_SP_BOX, box);
}

dgus_status_t dgus_dgusii_text_set_length(dgus_ui_t *ui,
                                          const dgus_dgusii_text_t *text,
                                          uint16_t byte_len)
{
    return dgus_dgusii_text_desc_write_u16(ui,
                                           text,
                                           DGUS_DGUSII_TEXT_SP_LENGTH,
                                           byte_len);
}

dgus_status_t dgus_dgusii_text_set_lib_id(dgus_ui_t *ui,
                                          const dgus_dgusii_text_t *text,
                                          uint16_t lib_id)
{
    return dgus_dgusii_text_desc_write_u16(ui,
                                           text,
                                           DGUS_DGUSII_TEXT_SP_LIB_ID,
                                           lib_id);
}

dgus_status_t dgus_dgusii_text_set_mode_spacing(dgus_ui_t *ui,
                                                const dgus_dgusii_text_t *text,
                                                uint8_t display_mode,
                                                uint8_t horizontal_spacing)
{
    return dgus_dgusii_text_desc_write_u16(ui,
                                           text,
                                           DGUS_DGUSII_TEXT_SP_MODE_HOR,
                                           dgus_pack_u8(display_mode,
                                                        horizontal_spacing));
}

dgus_status_t dgus_dgusii_text_set_vertical_spacing(dgus_ui_t *ui,
                                                    const dgus_dgusii_text_t *text,
                                                    uint8_t vertical_spacing)
{
    return dgus_dgusii_text_desc_write_u16(ui,
                                           text,
                                           DGUS_DGUSII_TEXT_SP_VER_RESERVED,
                                           dgus_pack_u8(vertical_spacing, 0u));
}
