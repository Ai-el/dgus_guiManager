#ifndef DGUS_TREND_CURVE_H
#define DGUS_TREND_CURVE_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* DGUS system-variable addresses for trend-curve buffer writes. */
#define DGUS_TREND_STATUS_BASE       0x0300u
#define DGUS_TREND_WRITE_START       0x0310u
#define DGUS_TREND_DATA_BASE         0x0312u
#define DGUS_TREND_MAX_CHANNELS      8u
#define DGUS_TREND_MAX_BLOCK_WORDS   0x6Eu

/* Offsets for trend-curve display (14.BIN type 0x20). */
#define DGUS_TREND_SP_MODE           0x00u
#define DGUS_TREND_SP_AREA           0x01u
#define DGUS_TREND_SP_Y_CENTER       0x05u
#define DGUS_TREND_SP_DATA_CENTER    0x06u
#define DGUS_TREND_SP_COLOR          0x07u
#define DGUS_TREND_SP_Y_SCALE        0x08u
#define DGUS_TREND_SP_CHANNEL_HOR    0x09u
#define DGUS_TREND_SP_PIXEL_SCALE    0x0Au

/* Trend curve direction: whether the latest samples enter from right or left. */
typedef enum {
    DGUS_TREND_LATEST_RIGHT = 0,
    DGUS_TREND_LATEST_LEFT = 1
} dgus_trend_dir_t;

/* Realtime trend-curve display. Samples are written to DGUS system variables.
 *
 * Parameters:
 * - sp is the descriptor base for type 0x20, or DGUS_SP_NONE.
 * - channel selects one of DGUS_TREND_MAX_CHANNELS source channels, 0..7.
 * - area is the pixel rectangle used for the curve window.
 * - scale_1_256 uses DGUS 1/256 scale units.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
    uint8_t channel;
} dgus_trend_curve_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_trend_curve_t, dgus_trend_curve);

/* Create a trend-curve display object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * sp: descriptor pointer for 14.BIN type 0x20, or DGUS_SP_NONE.
 * channel: default channel used by dgus_trend_curve_write_samples().
 */
dgus_trend_curve_t *dgus_trend_curve_create(dgus_page_t *page,
                                            uint16_t id,
                                            uint16_t sp,
                                            uint8_t channel);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_trend_curve_has_sp(const dgus_trend_curve_t *curve);

/* Write an arbitrary descriptor word for trend-curve display. */
dgus_status_t dgus_trend_curve_desc_write_u16(dgus_ui_t *ui,
                                              const dgus_trend_curve_t *curve,
                                              uint16_t offset,
                                              uint16_t value);

/* Set curve direction mode at SP+0. */
dgus_status_t dgus_trend_curve_set_direction(dgus_ui_t *ui,
                                             const dgus_trend_curve_t *curve,
                                             dgus_trend_dir_t direction);

/* Set curve window rectangle at SP+1. */
dgus_status_t dgus_trend_curve_set_area(dgus_ui_t *ui,
                                        const dgus_trend_curve_t *curve,
                                        dgus_rect_t area);

/* Configure Y center pixel and data-center value. */
dgus_status_t dgus_trend_curve_set_y_axis(dgus_ui_t *ui,
                                          const dgus_trend_curve_t *curve,
                                          uint16_t y_center,
                                          uint16_t data_center);

/* Configure curve RGB565 color. */
dgus_status_t dgus_trend_curve_set_color(dgus_ui_t *ui,
                                         const dgus_trend_curve_t *curve,
                                         uint16_t rgb565);

/* Configure Y-axis scale in 1/256 units. */
dgus_status_t dgus_trend_curve_set_y_scale(dgus_ui_t *ui,
                                           const dgus_trend_curve_t *curve,
                                           uint16_t scale_1_256);

/* Configure source channel and horizontal spacing.
 * channel: 0..7 trend channel.
 * horizontal_spacing: pixels between samples.
 */
dgus_status_t dgus_trend_curve_set_channel_spacing(dgus_ui_t *ui,
                                                   const dgus_trend_curve_t *curve,
                                                   uint8_t channel,
                                                   uint8_t horizontal_spacing);

/* Configure pixel scale, where 0..7 maps to 1x1..8x8 pixels. */
dgus_status_t dgus_trend_curve_set_pixel_scale(dgus_ui_t *ui,
                                               const dgus_trend_curve_t *curve,
                                               uint8_t pixel_scale);

/* Clear one trend channel by zeroing its valid-length status word.
 * This low-level helper is not tied to one trend object, so it cannot emit a
 * widget parameter callback. Prefer dgus_trend_curve_clear() when a curve
 * object is available.
 */
dgus_status_t dgus_trend_curve_clear_channel(dgus_ui_t *ui, uint8_t channel);

/* Clear the channel stored in this trend-curve object.
 * curve: trend object whose channel field selects the DGUS channel to clear.
 * Emits the curve's parameter callback after the status word is written.
 */
dgus_status_t dgus_trend_curve_clear(dgus_ui_t *ui,
                                     const dgus_trend_curve_t *curve);

/* Write one data block to the DGUS trend-curve buffer.
 * channel: 0..7 trend channel.
 * samples: raw sample words.
 * sample_count: must not exceed DGUS_TREND_MAX_BLOCK_WORDS.
 * This low-level helper is not tied to one trend object, so it cannot emit a
 * widget parameter callback. Prefer dgus_trend_curve_write_samples() when a
 * curve object is available.
 */
dgus_status_t dgus_trend_curve_write_channel(dgus_ui_t *ui,
                                             uint8_t channel,
                                             const uint16_t *samples,
                                             uint8_t sample_count);

/* Write samples using the channel stored in the curve object. */
dgus_status_t dgus_trend_curve_write_samples(dgus_ui_t *ui,
                                             const dgus_trend_curve_t *curve,
                                             const uint16_t *samples,
                                             uint8_t sample_count);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_TREND_CURVE_H */
