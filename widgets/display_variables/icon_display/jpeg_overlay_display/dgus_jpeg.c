#include "dgus_jpeg.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_jpeg_t, dgus_jpeg)
dgus_jpeg_t *dgus_jpeg_create(dgus_page_t *page,
                              uint16_t id,
                              uint16_t vp,
                              uint16_t sp)
{
    dgus_jpeg_t *jpeg;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    jpeg = (dgus_jpeg_t *)dgus_page_alloc(page, sizeof(*jpeg));
    if (jpeg == NULL) {
        return NULL;
    }
    dgus_widget_init_obj(&jpeg->obj, id, DGUS_OBJ_JPEG, vp, 4u);
    jpeg->sp = sp;
    if (dgus_page_add_obj(page, &jpeg->obj) != DGUS_OK) {
        return NULL;
    }

    return jpeg;
}

bool dgus_jpeg_has_sp(const dgus_jpeg_t *jpeg)
{
    return (jpeg != NULL) && dgus_widget_has_sp(jpeg->sp);
}

dgus_status_t dgus_jpeg_enable(dgus_ui_t *ui, const dgus_jpeg_t *jpeg)
{
    if (jpeg == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_write_u16_at(ui, &jpeg->obj, DGUS_JPEG_VP_ENABLE, 0x5AA5u);
}

dgus_status_t dgus_jpeg_disable(dgus_ui_t *ui, const dgus_jpeg_t *jpeg)
{
    if (jpeg == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_write_u16_at(ui, &jpeg->obj, DGUS_JPEG_VP_ENABLE, 0u);
}

dgus_status_t dgus_jpeg_set_buffer_words(dgus_ui_t *ui,
                                         const dgus_jpeg_t *jpeg,
                                         uint16_t word_count)
{
    if (jpeg == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_write_u16_at(ui, &jpeg->obj, DGUS_JPEG_VP_WORD_LENGTH, word_count);
}

dgus_status_t dgus_jpeg_write_data_words(dgus_ui_t *ui,
                                         const dgus_jpeg_t *jpeg,
                                         const uint16_t *words,
                                         uint8_t word_count)
{
    if (jpeg == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_write_words_at(ui, &jpeg->obj, DGUS_JPEG_VP_DATA, words, word_count);
}

dgus_status_t dgus_jpeg_desc_write_u16(dgus_ui_t *ui,
                                       const dgus_jpeg_t *jpeg,
                                       uint16_t offset,
                                       uint16_t value)
{
    if (!dgus_jpeg_has_sp(jpeg)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &jpeg->obj, jpeg->sp, offset, value);
}

dgus_status_t dgus_jpeg_set_pos(dgus_ui_t *ui,
                                const dgus_jpeg_t *jpeg,
                                int16_t x,
                                int16_t y)
{
    dgus_point_t point;

    if (!dgus_jpeg_has_sp(jpeg)) {
        return DGUS_ERR_ARG;
    }

    point.x = x;
    point.y = y;
    return dgus_obj_desc_write_point_at(ui, &jpeg->obj, jpeg->sp, DGUS_JPEG_SP_POS, point);
}

dgus_status_t dgus_jpeg_set_window(dgus_ui_t *ui,
                                   const dgus_jpeg_t *jpeg,
                                   uint16_t width,
                                   uint16_t height)
{
    uint16_t words[2];

    words[0] = width;
    words[1] = height;
    if (!dgus_jpeg_has_sp(jpeg)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_words_at(ui, &jpeg->obj, jpeg->sp, DGUS_JPEG_SP_WIDTH, words, 2u);
}

dgus_status_t dgus_jpeg_set_brightness_mode(dgus_ui_t *ui,
                                            const dgus_jpeg_t *jpeg,
                                            uint8_t brightness,
                                            dgus_jpeg_display_mode_t mode)
{
    return dgus_jpeg_desc_write_u16(ui,
                                    jpeg,
                                    DGUS_JPEG_SP_DIM_MODE,
                                    dgus_pack_u8(brightness, (uint8_t)mode));
}

dgus_status_t dgus_jpeg_set_filter_page(dgus_ui_t *ui,
                                        const dgus_jpeg_t *jpeg,
                                        uint8_t filter_strength,
                                        uint8_t vp_page)
{
    return dgus_jpeg_desc_write_u16(ui,
                                    jpeg,
                                    DGUS_JPEG_SP_FILTER_PAGE,
                                    dgus_pack_u8(filter_strength, vp_page));
}
