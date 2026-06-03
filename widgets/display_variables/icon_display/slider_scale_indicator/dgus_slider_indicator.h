#ifndef DGUS_SLIDER_INDICATOR_H
#define DGUS_SLIDER_INDICATOR_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Offsets for slider scale indicator display (14.BIN type 0x02). */
#define DGUS_SLIDER_SP_VP            0x00u
#define DGUS_SLIDER_SP_V_BEGIN       0x01u
#define DGUS_SLIDER_SP_V_END         0x02u
#define DGUS_SLIDER_SP_X_BEGIN       0x03u
#define DGUS_SLIDER_SP_X_END         0x04u
#define DGUS_SLIDER_SP_ICON_ID       0x05u
#define DGUS_SLIDER_SP_Y             0x06u
#define DGUS_SLIDER_SP_ADJ_MODE      0x07u
#define DGUS_SLIDER_SP_LIB_ICON_MODE 0x08u
#define DGUS_SLIDER_SP_DATA_LAYER    0x09u

/* Slider orientation stored in the adjustment-mode descriptor field. */
typedef enum {
    DGUS_SLIDER_HORIZONTAL = 0,
    DGUS_SLIDER_VERTICAL = 1
} dgus_slider_dir_t;

/* Slider scale indicator. The VP value is mapped to a screen-axis coordinate.
 *
 * Parameters:
 * - vp stores the source value.
 * - begin_value/end_value define the numeric value range.
 * - begin_axis/end_axis define the X range for horizontal mode or Y range for
 *   vertical mode.
 * - fixed_axis is the non-moving coordinate, Y for horizontal or X for vertical.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
} dgus_slider_indicator_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_slider_indicator_t, dgus_slider_indicator);

/* Create a slider scale indicator object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS VP storing the slider source value.
 * sp: descriptor pointer for 14.BIN type 0x02, or DGUS_SP_NONE.
 */
dgus_slider_indicator_t *dgus_slider_indicator_create(dgus_page_t *page,
                                                      uint16_t id,
                                                      uint16_t vp,
                                                      uint16_t sp);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_slider_indicator_has_sp(const dgus_slider_indicator_t *slider);

/* Write the indicator source value to the object's VP.
 * value: mapped between begin_value and end_value.
 */
dgus_status_t dgus_slider_indicator_set_value(dgus_ui_t *ui,
                                              const dgus_slider_indicator_t *slider,
                                              uint16_t value);

/* Write an arbitrary descriptor word for slider indicator display. */
dgus_status_t dgus_slider_indicator_desc_write_u16(dgus_ui_t *ui,
                                                   const dgus_slider_indicator_t *slider,
                                                   uint16_t offset,
                                                   uint16_t value);

/* Configure the data value range mapped to the scale.
 * begin_value/end_value: inclusive value limits in VP units.
 */
dgus_status_t dgus_slider_indicator_set_value_range(dgus_ui_t *ui,
                                                    const dgus_slider_indicator_t *slider,
                                                    uint16_t begin_value,
                                                    uint16_t end_value);

/* Configure the screen-axis range mapped to the scale.
 * begin_axis/end_axis: pixel coordinates on the moving axis.
 */
dgus_status_t dgus_slider_indicator_set_axis_range(dgus_ui_t *ui,
                                                   const dgus_slider_indicator_t *slider,
                                                   uint16_t begin_axis,
                                                   uint16_t end_axis);

/* Configure indicator icon ID and its fixed axis coordinate.
 * icon_id: icon displayed as the slider marker.
 * fixed_axis: Y in horizontal mode, X in vertical mode.
 */
dgus_status_t dgus_slider_indicator_set_icon(dgus_ui_t *ui,
                                             const dgus_slider_indicator_t *slider,
                                             uint16_t icon_id,
                                             uint16_t fixed_axis);

/* Configure horizontal or vertical scale mode. */
dgus_status_t dgus_slider_indicator_set_direction(dgus_ui_t *ui,
                                                  const dgus_slider_indicator_t *slider,
                                                  dgus_slider_dir_t dir);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_SLIDER_INDICATOR_H */
