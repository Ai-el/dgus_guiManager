#include "dgus_jpeg_pan.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_jpeg_pan_t, dgus_jpeg_pan)
dgus_jpeg_pan_t *dgus_jpeg_pan_create(dgus_page_t *page,
                                      uint16_t id,
                                      uint16_t vp,
                                      uint16_t sp,
                                      uint8_t word_count)
{
    dgus_jpeg_pan_t *pan;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    pan = (dgus_jpeg_pan_t *)dgus_page_alloc(page, sizeof(*pan));
    if (pan == NULL) {
        return NULL;
    }
    dgus_widget_init_obj(&pan->obj, id, DGUS_OBJ_JPEG_PAN, vp, word_count);
    pan->sp = sp;
    pan->word_count = word_count;
    if (dgus_page_add_obj(page, &pan->obj) != DGUS_OK) {
        return NULL;
    }

    return pan;
}

bool dgus_jpeg_pan_has_sp(const dgus_jpeg_pan_t *pan)
{
    return (pan != NULL) && dgus_widget_has_sp(pan->sp);
}

dgus_status_t dgus_jpeg_pan_write_words(dgus_ui_t *ui,
                                        const dgus_jpeg_pan_t *pan,
                                        const uint16_t *words,
                                        uint8_t word_count)
{
    if (pan == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_write_words(ui, &pan->obj, words, word_count);
}

dgus_status_t dgus_jpeg_pan_desc_write_u16(dgus_ui_t *ui,
                                           const dgus_jpeg_pan_t *pan,
                                           uint16_t offset,
                                           uint16_t value)
{
    if (!dgus_jpeg_pan_has_sp(pan)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &pan->obj, pan->sp, offset, value);
}

dgus_status_t dgus_jpeg_pan_desc_write_words(dgus_ui_t *ui,
                                             const dgus_jpeg_pan_t *pan,
                                             uint16_t offset,
                                             const uint16_t *words,
                                             uint8_t word_count)
{
    if (!dgus_jpeg_pan_has_sp(pan)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_words_at(ui, &pan->obj, pan->sp, offset, words, word_count);
}
