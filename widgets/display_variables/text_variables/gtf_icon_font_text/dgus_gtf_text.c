#include "dgus_gtf_text.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_gtf_text_t, dgus_gtf_text)
dgus_gtf_text_t *dgus_gtf_text_create(dgus_page_t *page,
                                      uint16_t id,
                                      uint16_t vp,
                                      uint16_t sp,
                                      uint8_t max_chars)
{
    dgus_gtf_text_t *text;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    text = (dgus_gtf_text_t *)dgus_page_alloc(page, sizeof(*text));
    if (text == NULL) {
        return NULL;
    }
    dgus_widget_init_obj(&text->obj,
                         id,
                         DGUS_OBJ_GTF_TEXT,
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

bool dgus_gtf_text_has_sp(const dgus_gtf_text_t *text)
{
    return (text != NULL) && dgus_widget_has_sp(text->sp);
}

void dgus_gtf_text_set_terminate(dgus_gtf_text_t *text, bool terminate)
{
    if (text != NULL) {
        text->terminate = terminate;
    }
}

dgus_status_t dgus_gtf_text_set_text_ascii(dgus_ui_t *ui,
                                           const dgus_gtf_text_t *text,
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

dgus_status_t dgus_gtf_text_desc_write_u16(dgus_ui_t *ui,
                                           const dgus_gtf_text_t *text,
                                           uint16_t offset,
                                           uint16_t value)
{
    if (!dgus_gtf_text_has_sp(text)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &text->obj, text->sp, offset, value);
}

dgus_status_t dgus_gtf_text_set_desc_vp(dgus_ui_t *ui,
                                        const dgus_gtf_text_t *text,
                                        uint16_t vp)
{
    return dgus_gtf_text_desc_write_u16(ui, text, DGUS_GTF_TEXT_SP_VP, vp);
}

dgus_status_t dgus_gtf_text_hide(dgus_ui_t *ui,
                                 const dgus_gtf_text_t *text)
{
    return dgus_gtf_text_set_desc_vp(ui, text, 0xFF00u);
}

dgus_status_t dgus_gtf_text_show(dgus_ui_t *ui,
                                 const dgus_gtf_text_t *text)
{
    if (text == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_gtf_text_set_desc_vp(ui, text, text->obj.vp);
}

dgus_status_t dgus_gtf_text_set_ids(dgus_ui_t *ui,
                                    const dgus_gtf_text_t *text,
                                    uint16_t gtf_id,
                                    uint16_t font_id)
{
    uint16_t words[2];

    words[0] = gtf_id;
    words[1] = font_id;
    if (!dgus_gtf_text_has_sp(text)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_words_at(ui, &text->obj, text->sp, DGUS_GTF_TEXT_SP_GTF_ID, words, 2u);
}

dgus_status_t dgus_gtf_text_set_pos(dgus_ui_t *ui,
                                    const dgus_gtf_text_t *text,
                                    int16_t x,
                                    int16_t y)
{
    dgus_point_t point;

    if (!dgus_gtf_text_has_sp(text)) {
        return DGUS_ERR_ARG;
    }

    point.x = x;
    point.y = y;
    return dgus_obj_desc_write_point_at(ui, &text->obj, text->sp, DGUS_GTF_TEXT_SP_POS, point);
}

dgus_status_t dgus_gtf_text_set_box(dgus_ui_t *ui,
                                    const dgus_gtf_text_t *text,
                                    dgus_rect_t box)
{
    if (!dgus_gtf_text_has_sp(text)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_rect_at(ui, &text->obj, text->sp, DGUS_GTF_TEXT_SP_BOX, box);
}

dgus_status_t dgus_gtf_text_set_spacing(dgus_ui_t *ui,
                                        const dgus_gtf_text_t *text,
                                        uint8_t horizontal,
                                        uint8_t vertical)
{
    return dgus_gtf_text_desc_write_u16(ui,
                                        text,
                                        DGUS_GTF_TEXT_SP_SPACING,
                                        dgus_pack_u8(horizontal, vertical));
}

dgus_status_t dgus_gtf_text_set_display_mode(dgus_ui_t *ui,
                                             const dgus_gtf_text_t *text,
                                             uint8_t display_mode)
{
    return dgus_gtf_text_desc_write_u16(ui,
                                        text,
                                        DGUS_GTF_TEXT_SP_DISPLAY_MODE,
                                        dgus_pack_u8(display_mode, 0u));
}
