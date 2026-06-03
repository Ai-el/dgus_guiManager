#ifndef DGUS_ICON_ROTATE_H
#define DGUS_ICON_ROTATE_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Offsets for icon rotation display (14.BIN type 0x05). */
#define DGUS_ICON_ROTATE_SP_VP           0x00u
#define DGUS_ICON_ROTATE_SP_ICON_ID      0x01u
#define DGUS_ICON_ROTATE_SP_ICON_CENTER  0x02u
#define DGUS_ICON_ROTATE_SP_SCREEN_CENTER 0x04u
#define DGUS_ICON_ROTATE_SP_V_BEGIN      0x06u
#define DGUS_ICON_ROTATE_SP_V_END        0x07u
#define DGUS_ICON_ROTATE_SP_ANGLE_BEGIN  0x08u
#define DGUS_ICON_ROTATE_SP_ANGLE_END    0x09u
#define DGUS_ICON_ROTATE_SP_MODE_LIB     0x0Au
#define DGUS_ICON_ROTATE_SP_ICON_MODE    0x0Bu

/* Icon rotation display. VP value is mapped to an angle range.
 *
 * Parameters:
 * - vp stores the rotation source value.
 * - icon_center is the rotation center inside the icon bitmap.
 * - screen_center is the fixed center point on screen.
 * - angles use 0.5 degree units; 720 means 360 degrees.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
} dgus_icon_rotate_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_icon_rotate_t, dgus_icon_rotate);

/* Create an icon rotation display object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS VP storing the rotation source value.
 * sp: descriptor pointer for 14.BIN type 0x05, or DGUS_SP_NONE.
 */
dgus_icon_rotate_t *dgus_icon_rotate_create(dgus_page_t *page,
                                            uint16_t id,
                                            uint16_t vp,
                                            uint16_t sp);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_icon_rotate_has_sp(const dgus_icon_rotate_t *rot);

/* Write the rotation source value to the object's VP. */
dgus_status_t dgus_icon_rotate_set_value(dgus_ui_t *ui,
                                         const dgus_icon_rotate_t *rot,
                                         uint16_t value);

/* Write an arbitrary descriptor word for icon rotation display. */
dgus_status_t dgus_icon_rotate_desc_write_u16(dgus_ui_t *ui,
                                              const dgus_icon_rotate_t *rot,
                                              uint16_t offset,
                                              uint16_t value);

/* Configure icon ID and icon library.
 * icon_id: icon bitmap to rotate.
 * icon_lib: DGUS icon library id.
 */
dgus_status_t dgus_icon_rotate_set_icon(dgus_ui_t *ui,
                                        const dgus_icon_rotate_t *rot,
                                        uint16_t icon_id,
                                        uint8_t icon_lib);

/* Configure icon-center and screen-center rotation points. */
dgus_status_t dgus_icon_rotate_set_centers(dgus_ui_t *ui,
                                           const dgus_icon_rotate_t *rot,
                                           dgus_point_t icon_center,
                                           dgus_point_t screen_center);

/* Configure source value range mapped to the angle range. */
dgus_status_t dgus_icon_rotate_set_value_range(dgus_ui_t *ui,
                                               const dgus_icon_rotate_t *rot,
                                               uint16_t begin_value,
                                               uint16_t end_value);

/* Configure angle range in 0.5 degree units. */
dgus_status_t dgus_icon_rotate_set_angle_range(dgus_ui_t *ui,
                                               const dgus_icon_rotate_t *rot,
                                               uint16_t begin_angle_0_5deg,
                                               uint16_t end_angle_0_5deg);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_ICON_ROTATE_H */
