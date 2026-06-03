#include "dgus_trend_curve.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_trend_curve_t, dgus_trend_curve)
dgus_trend_curve_t *dgus_trend_curve_create(dgus_page_t *page,
                                            uint16_t id,
                                            uint16_t sp,
                                            uint8_t channel)
{
    dgus_trend_curve_t *curve;

    if ((page == NULL) || (page->object_count >= page->object_capacity) ||
        (channel >= DGUS_TREND_MAX_CHANNELS)) {
        return NULL;
    }
    curve = (dgus_trend_curve_t *)dgus_page_alloc(page, sizeof(*curve));
    if (curve == NULL) {
        return NULL;
    }
    dgus_widget_init_obj(&curve->obj,
                         id,
                         DGUS_OBJ_TREND_CURVE,
                         DGUS_TREND_STATUS_BASE,
                         (uint8_t)(DGUS_TREND_WRITE_START -
                                   DGUS_TREND_STATUS_BASE +
                                   3u + DGUS_TREND_MAX_BLOCK_WORDS));
    curve->sp = sp;
    curve->channel = channel;
    if (dgus_page_add_obj(page, &curve->obj) != DGUS_OK) {
        return NULL;
    }

    return curve;
}

bool dgus_trend_curve_has_sp(const dgus_trend_curve_t *curve)
{
    return (curve != NULL) && dgus_widget_has_sp(curve->sp);
}

dgus_status_t dgus_trend_curve_desc_write_u16(dgus_ui_t *ui,
                                              const dgus_trend_curve_t *curve,
                                              uint16_t offset,
                                              uint16_t value)
{
    if (!dgus_trend_curve_has_sp(curve)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &curve->obj, curve->sp, offset, value);
}

dgus_status_t dgus_trend_curve_set_direction(dgus_ui_t *ui,
                                             const dgus_trend_curve_t *curve,
                                             dgus_trend_dir_t direction)
{
    return dgus_trend_curve_desc_write_u16(ui,
                                           curve,
                                           DGUS_TREND_SP_MODE,
                                           dgus_pack_u8((uint8_t)direction, 0u));
}

dgus_status_t dgus_trend_curve_set_area(dgus_ui_t *ui,
                                        const dgus_trend_curve_t *curve,
                                        dgus_rect_t area)
{
    if (!dgus_trend_curve_has_sp(curve)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_rect_at(ui, &curve->obj, curve->sp, DGUS_TREND_SP_AREA, area);
}

dgus_status_t dgus_trend_curve_set_y_axis(dgus_ui_t *ui,
                                          const dgus_trend_curve_t *curve,
                                          uint16_t y_center,
                                          uint16_t data_center)
{
    uint16_t words[2];

    words[0] = y_center;
    words[1] = data_center;
    if (!dgus_trend_curve_has_sp(curve)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_words_at(ui, &curve->obj, curve->sp, DGUS_TREND_SP_Y_CENTER, words, 2u);
}

dgus_status_t dgus_trend_curve_set_color(dgus_ui_t *ui,
                                         const dgus_trend_curve_t *curve,
                                         uint16_t rgb565)
{
    return dgus_trend_curve_desc_write_u16(ui,
                                           curve,
                                           DGUS_TREND_SP_COLOR,
                                           rgb565);
}

dgus_status_t dgus_trend_curve_set_y_scale(dgus_ui_t *ui,
                                           const dgus_trend_curve_t *curve,
                                           uint16_t scale_1_256)
{
    return dgus_trend_curve_desc_write_u16(ui,
                                           curve,
                                           DGUS_TREND_SP_Y_SCALE,
                                           scale_1_256);
}

dgus_status_t dgus_trend_curve_set_channel_spacing(dgus_ui_t *ui,
                                                   const dgus_trend_curve_t *curve,
                                                   uint8_t channel,
                                                   uint8_t horizontal_spacing)
{
    if (channel >= DGUS_TREND_MAX_CHANNELS) {
        return DGUS_ERR_ARG;
    }

    return dgus_trend_curve_desc_write_u16(ui,
                                           curve,
                                           DGUS_TREND_SP_CHANNEL_HOR,
                                           dgus_pack_u8(channel,
                                                        horizontal_spacing));
}

dgus_status_t dgus_trend_curve_set_pixel_scale(dgus_ui_t *ui,
                                               const dgus_trend_curve_t *curve,
                                               uint8_t pixel_scale)
{
    if (pixel_scale > 7u) {
        return DGUS_ERR_ARG;
    }

    return dgus_trend_curve_desc_write_u16(ui,
                                           curve,
                                           DGUS_TREND_SP_PIXEL_SCALE,
                                           dgus_pack_u8(pixel_scale, 0u));
}

dgus_status_t dgus_trend_curve_clear_channel(dgus_ui_t *ui, uint8_t channel)
{
    if ((ui == NULL) || (ui->dev == NULL) ||
        (channel >= DGUS_TREND_MAX_CHANNELS)) {
        return DGUS_ERR_ARG;
    }

    return dgus_write_u16(ui->dev,
                          (uint16_t)(DGUS_TREND_STATUS_BASE + 1u +
                                     ((uint16_t)channel * 2u)),
                          0u);
}

dgus_status_t dgus_trend_curve_clear(dgus_ui_t *ui,
                                     const dgus_trend_curve_t *curve)
{
    uint16_t offset;

    if ((ui == NULL) || (ui->dev == NULL) || (curve == NULL)) {
        return DGUS_ERR_ARG;
    }

    offset = (uint16_t)(1u + ((uint16_t)curve->channel * 2u));
    return dgus_obj_write_u16_at(ui, &curve->obj, offset, 0u);
}

dgus_status_t dgus_trend_curve_write_channel(dgus_ui_t *ui,
                                             uint8_t channel,
                                             const uint16_t *samples,
                                             uint8_t sample_count)
{
    uint16_t words[2u + 1u + DGUS_TREND_MAX_BLOCK_WORDS];
    uint8_t i;

    if ((ui == NULL) || (ui->dev == NULL) || (samples == NULL) ||
        (sample_count == 0u) || (sample_count > DGUS_TREND_MAX_BLOCK_WORDS) ||
        (channel >= DGUS_TREND_MAX_CHANNELS)) {
        return DGUS_ERR_ARG;
    }

    words[0] = 0x5AA5u;
    words[1] = dgus_pack_u8(1u, 0u);
    words[2] = dgus_pack_u8(channel, sample_count);
    for (i = 0u; i < sample_count; i++) {
        words[3u + i] = samples[i];
    }

    return dgus_write_words(ui->dev,
                            DGUS_TREND_WRITE_START,
                            words,
                            (uint8_t)(3u + sample_count));
}

dgus_status_t dgus_trend_curve_write_samples(dgus_ui_t *ui,
                                             const dgus_trend_curve_t *curve,
                                             const uint16_t *samples,
                                             uint8_t sample_count)
{
    uint16_t words[2u + 1u + DGUS_TREND_MAX_BLOCK_WORDS];
    uint8_t i;

    if (curve == NULL) {
        return DGUS_ERR_ARG;
    }
    if ((samples == NULL) || (sample_count == 0u) ||
        (sample_count > DGUS_TREND_MAX_BLOCK_WORDS)) {
        return DGUS_ERR_ARG;
    }

    words[0] = 0x5AA5u;
    words[1] = dgus_pack_u8(1u, 0u);
    words[2] = dgus_pack_u8(curve->channel, sample_count);
    for (i = 0u; i < sample_count; i++) {
        words[3u + i] = samples[i];
    }

    return dgus_obj_write_words_at(ui,
                                   &curve->obj,
                                   (uint16_t)(DGUS_TREND_WRITE_START -
                                              DGUS_TREND_STATUS_BASE),
                                   words,
                                   (uint8_t)(3u + sample_count));
}
