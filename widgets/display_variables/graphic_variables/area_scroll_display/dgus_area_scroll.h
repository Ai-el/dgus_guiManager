#ifndef DGUS_AREA_SCROLL_H
#define DGUS_AREA_SCROLL_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Offsets for area scroll display (14.BIN type 0x24). */
#define DGUS_AREA_SCROLL_SP_VP       0x00u
#define DGUS_AREA_SCROLL_SP_START    0x01u
#define DGUS_AREA_SCROLL_SP_END      0x03u
#define DGUS_AREA_SCROLL_SP_SPEED    0x05u
#define DGUS_AREA_SCROLL_SP_DIR      0x06u

/* Area scroll display.
 *
 * Parameters:
 * - vp is the display variable VP configured in the DGUS project.
 * - sp is the descriptor base for type 0x24, or DGUS_SP_NONE.
 * - area is the scrolling rectangle in pixels.
 * - speed is the DGUS scroll-speed word; dir uses dgus_scroll_dir_t.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
} dgus_area_scroll_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_area_scroll_t, dgus_area_scroll);

/* Create an area scroll display object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS VP linked by the area scroll descriptor.
 * sp: descriptor pointer for 14.BIN type 0x24, or DGUS_SP_NONE.
 */
dgus_area_scroll_t *dgus_area_scroll_create(dgus_page_t *page,
                                            uint16_t id,
                                            uint16_t vp,
                                            uint16_t sp);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_area_scroll_has_sp(const dgus_area_scroll_t *scroll);

/* Write an arbitrary descriptor word for area scroll display. */
dgus_status_t dgus_area_scroll_desc_write_u16(dgus_ui_t *ui,
                                              const dgus_area_scroll_t *scroll,
                                              uint16_t offset,
                                              uint16_t value);

/* Set the scrolling rectangle at SP+1..SP+4. */
dgus_status_t dgus_area_scroll_set_area(dgus_ui_t *ui,
                                        const dgus_area_scroll_t *scroll,
                                        dgus_rect_t area);

/* Set scrolling speed at SP+5. */
dgus_status_t dgus_area_scroll_set_speed(dgus_ui_t *ui,
                                         const dgus_area_scroll_t *scroll,
                                         uint16_t speed);

/* Set scrolling direction at SP+6. */
dgus_status_t dgus_area_scroll_set_direction(dgus_ui_t *ui,
                                             const dgus_area_scroll_t *scroll,
                                             dgus_scroll_dir_t dir);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_AREA_SCROLL_H */
