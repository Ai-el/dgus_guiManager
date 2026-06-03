#include "dgus_text.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_text_t, dgus_text)
DGUS_WIDGET_DEFINE_CALLBACKS(dgus_label_t, dgus_label)
dgus_text_t *dgus_text_create(dgus_page_t *page,
                              uint16_t id,
                              uint16_t vp,
                              uint16_t sp,
                              uint8_t max_chars)
{
    dgus_text_t *text;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    text = (dgus_text_t *)dgus_page_alloc(page, sizeof(dgus_text_t));
    if (text == NULL) {
        return NULL;
    }
    dgus_widget_init_obj(&text->obj,
                         id,
                         DGUS_OBJ_TEXT,
                         vp,
                         (uint8_t)(max_chars + 1u));
    text->sp = sp;
    text->max_chars = max_chars;
    text->terminate = true;
    if (dgus_page_add_obj(page, &text->obj) != DGUS_OK) {
        return NULL;
    }

    return text;
}

dgus_label_t *dgus_label_create(dgus_page_t *page,
                                uint16_t id,
                                uint16_t vp,
                                uint16_t sp,
                                uint8_t max_chars)
{
    return dgus_text_create(page, id, vp, sp, max_chars);
}

bool dgus_text_has_sp(const dgus_text_t *text)
{
    return (text != NULL) && dgus_widget_has_sp(text->sp);
}

void dgus_text_set_terminate(dgus_text_t *text, bool terminate)
{
    if (text != NULL) {
        text->terminate = terminate;
    }
}

dgus_status_t dgus_text_set_text_ascii(dgus_ui_t *ui,
                                       const dgus_text_t *text,
                                       const char *value)
{
    if (text == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_set_text_ascii(ui,
                                   &text->obj,
                                   value,
                                   text->max_chars,
                                   text->terminate);
}

dgus_status_t dgus_label_set_text_ascii(dgus_ui_t *ui,
                                        const dgus_label_t *label,
                                        const char *value)
{
    return dgus_text_set_text_ascii(ui, label, value);
}

dgus_status_t dgus_text_desc_write_u16(dgus_ui_t *ui,
                                       const dgus_text_t *text,
                                       uint16_t offset,
                                       uint16_t value)
{
    if (!dgus_text_has_sp(text)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &text->obj, text->sp, offset, value);
}

dgus_status_t dgus_text_desc_write_words(dgus_ui_t *ui,
                                         const dgus_text_t *text,
                                         uint16_t offset,
                                         const uint16_t *words,
                                         uint8_t word_count)
{
    if (!dgus_text_has_sp(text)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_words_at(ui, &text->obj, text->sp, offset, words, word_count);
}

dgus_status_t dgus_text_set_desc_vp(dgus_ui_t *ui,
                                    const dgus_text_t *text,
                                    uint16_t vp)
{
    return dgus_text_desc_write_u16(ui, text, DGUS_TEXT_SP_VP, vp);
}

dgus_status_t dgus_text_hide(dgus_ui_t *ui, const dgus_text_t *text)
{
    return dgus_text_set_desc_vp(ui, text, 0xFF00u);
}

dgus_status_t dgus_text_show(dgus_ui_t *ui, const dgus_text_t *text)
{
    if (text == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_text_set_desc_vp(ui, text, text->obj.vp);
}

dgus_status_t dgus_text_set_pos(dgus_ui_t *ui,
                                const dgus_text_t *text,
                                int16_t x,
                                int16_t y)
{
    dgus_point_t point;

    if (!dgus_text_has_sp(text)) {
        return DGUS_ERR_ARG;
    }

    point.x = x;
    point.y = y;

    return dgus_obj_desc_write_point_at(ui, &text->obj, text->sp, DGUS_TEXT_SP_POS, point);
}

dgus_status_t dgus_text_set_color(dgus_ui_t *ui,
                                  const dgus_text_t *text,
                                  uint16_t rgb565)
{
    return dgus_text_desc_write_u16(ui, text, DGUS_TEXT_SP_COLOR, rgb565);
}

dgus_status_t dgus_text_set_box(dgus_ui_t *ui,
                                const dgus_text_t *text,
                                dgus_rect_t box)
{
    if (!dgus_text_has_sp(text)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_rect_at(ui, &text->obj, text->sp, DGUS_TEXT_SP_BOX, box);
}

dgus_status_t dgus_text_set_length(dgus_ui_t *ui,
                                   const dgus_text_t *text,
                                   uint16_t byte_len)
{
    return dgus_text_desc_write_u16(ui, text, DGUS_TEXT_SP_LENGTH, byte_len);
}

dgus_status_t dgus_text_set_font_ids(dgus_ui_t *ui,
                                     const dgus_text_t *text,
                                     uint8_t ascii_font,
                                     uint8_t non_ascii_font)
{
    return dgus_text_desc_write_u16(ui,
                                    text,
                                    DGUS_TEXT_SP_FONT_ID,
                                    dgus_pack_u8(ascii_font, non_ascii_font));
}

dgus_status_t dgus_text_set_font_dots(dgus_ui_t *ui,
                                      const dgus_text_t *text,
                                      uint8_t x_dots,
                                      uint8_t y_dots)
{
    return dgus_text_desc_write_u16(ui,
                                    text,
                                    DGUS_TEXT_SP_FONT_DOTS,
                                    dgus_pack_u8(x_dots, y_dots));
}
