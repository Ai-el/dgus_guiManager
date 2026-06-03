#ifndef DGUS_BIT_ICON_H
#define DGUS_BIT_ICON_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Offsets for bit variable icon display (14.BIN type 0x06). */
#define DGUS_BIT_ICON_SP_VP          0x00u
#define DGUS_BIT_ICON_SP_AUX_VP      0x01u
#define DGUS_BIT_ICON_SP_ACTIVE_BITS 0x02u
#define DGUS_BIT_ICON_SP_DISPLAY     0x03u
#define DGUS_BIT_ICON_SP_ICON_MODE   0x04u
#define DGUS_BIT_ICON_SP_ICON0_START 0x05u
#define DGUS_BIT_ICON_SP_ICON0_END   0x06u
#define DGUS_BIT_ICON_SP_ICON1_START 0x07u
#define DGUS_BIT_ICON_SP_ICON1_END   0x08u
#define DGUS_BIT_ICON_SP_POS         0x09u
#define DGUS_BIT_ICON_SP_STEP        0x0Bu

/* Bit-variable icon display. Each active bit selects a 0-state or 1-state icon.
 *
 * Parameters:
 * - vp stores the source bit mask.
 * - mask selects which bits are displayed.
 * - dx/dy are pixel spacing between per-bit icons.
 * - icon0_* and icon1_* are icon ID ranges for cleared/set bits.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
} dgus_bit_icon_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_bit_icon_t, dgus_bit_icon);

/* Create a bit-variable icon display object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS VP storing the bit mask.
 * sp: descriptor pointer for 14.BIN type 0x06, or DGUS_SP_NONE.
 */
dgus_bit_icon_t *dgus_bit_icon_create(dgus_page_t *page,
                                      uint16_t id,
                                      uint16_t vp,
                                      uint16_t sp);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_bit_icon_has_sp(const dgus_bit_icon_t *icon);

/* Write the bit mask value to the object's VP. */
dgus_status_t dgus_bit_icon_set_bits(dgus_ui_t *ui,
                                     const dgus_bit_icon_t *icon,
                                     uint16_t bits);

/* Write an arbitrary descriptor word for bit icon display. */
dgus_status_t dgus_bit_icon_desc_write_u16(dgus_ui_t *ui,
                                           const dgus_bit_icon_t *icon,
                                           uint16_t offset,
                                           uint16_t value);

/* Configure which bits are displayed at SP+2.
 * mask: bit mask, where bit 0 enables display of source bit 0, etc.
 */
dgus_status_t dgus_bit_icon_set_active_bits(dgus_ui_t *ui,
                                            const dgus_bit_icon_t *icon,
                                            uint16_t mask);

/* Move the first bit icon by writing X,Y to SP+9. */
dgus_status_t dgus_bit_icon_set_pos(dgus_ui_t *ui,
                                    const dgus_bit_icon_t *icon,
                                    int16_t x,
                                    int16_t y);

/* Configure per-bit icon step in X,Y. */
dgus_status_t dgus_bit_icon_set_step(dgus_ui_t *ui,
                                     const dgus_bit_icon_t *icon,
                                     int16_t dx,
                                     int16_t dy);

/* Configure icon IDs used for bit 0 and bit 1 states. */
dgus_status_t dgus_bit_icon_set_icon_ids(dgus_ui_t *ui,
                                         const dgus_bit_icon_t *icon,
                                         uint16_t icon0_start,
                                         uint16_t icon0_end,
                                         uint16_t icon1_start,
                                         uint16_t icon1_end);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_BIT_ICON_H */
