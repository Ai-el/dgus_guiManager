#ifndef DGUS_AREA_BRIGHTNESS_H
#define DGUS_AREA_BRIGHTNESS_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Offsets for area brightness display (14.BIN type 0x26). */
#define DGUS_AREA_BRIGHTNESS_SP_VP      0x00u
#define DGUS_AREA_BRIGHTNESS_SP_START   0x01u
#define DGUS_AREA_BRIGHTNESS_SP_END     0x03u

/* Area brightness display.
 *
 * Parameters:
 * - vp stores the brightness value, normally 0x0000..0x0100.
 * - sp is the descriptor base for type 0x26, or DGUS_SP_NONE.
 * - rect is the controlled screen rectangle.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
} dgus_area_brightness_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_area_brightness_t, dgus_area_brightness);

/* Create an area brightness display object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS VP storing brightness value.
 * sp: descriptor pointer for 14.BIN type 0x26, or DGUS_SP_NONE.
 */
dgus_area_brightness_t *dgus_area_brightness_create(dgus_page_t *page,
                                                    uint16_t id,
                                                    uint16_t vp,
                                                    uint16_t sp);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_area_brightness_has_sp(const dgus_area_brightness_t *area);

/* Write brightness value, 0x0000..0x0100, to the VP. */
dgus_status_t dgus_area_brightness_set_value(dgus_ui_t *ui,
                                             const dgus_area_brightness_t *area,
                                             uint16_t value_1_256);

/* Write an arbitrary descriptor word for area brightness display. */
dgus_status_t dgus_area_brightness_desc_write_u16(dgus_ui_t *ui,
                                                  const dgus_area_brightness_t *area,
                                                  uint16_t offset,
                                                  uint16_t value);

/* Set the brightness-controlled rectangle at SP+1..SP+4. */
dgus_status_t dgus_area_brightness_set_area(dgus_ui_t *ui,
                                            const dgus_area_brightness_t *area,
                                            dgus_rect_t rect);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_AREA_BRIGHTNESS_H */
