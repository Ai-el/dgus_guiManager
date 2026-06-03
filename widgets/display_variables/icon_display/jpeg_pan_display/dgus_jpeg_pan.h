#ifndef DGUS_JPEG_PAN_H
#define DGUS_JPEG_PAN_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* JPEG pan display (14.BIN type 0x07). The manual exposes this mainly through
 * DGUS project configuration; descriptor access is still typed to this object.
 *
 * Parameters:
 * - vp is the control/data VP block used by the DGUS project.
 * - sp is the descriptor base for type 0x07, or DGUS_SP_NONE.
 * - word_count is the number of VP control words reserved for this block.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
    uint8_t word_count;
} dgus_jpeg_pan_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_jpeg_pan_t, dgus_jpeg_pan);

/* Create a JPEG pan display object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS VP for control words.
 * sp: descriptor pointer for 14.BIN type 0x07, or DGUS_SP_NONE.
 * word_count: VP word span used for routing and writes.
 */
dgus_jpeg_pan_t *dgus_jpeg_pan_create(dgus_page_t *page,
                                      uint16_t id,
                                      uint16_t vp,
                                      uint16_t sp,
                                      uint8_t word_count);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_jpeg_pan_has_sp(const dgus_jpeg_pan_t *pan);

/* Write control words to the object's VP. */
dgus_status_t dgus_jpeg_pan_write_words(dgus_ui_t *ui,
                                        const dgus_jpeg_pan_t *pan,
                                        const uint16_t *words,
                                        uint8_t word_count);

/* Write an arbitrary descriptor word for JPEG pan display. */
dgus_status_t dgus_jpeg_pan_desc_write_u16(dgus_ui_t *ui,
                                           const dgus_jpeg_pan_t *pan,
                                           uint16_t offset,
                                           uint16_t value);

/* Write arbitrary descriptor words for JPEG pan display. */
dgus_status_t dgus_jpeg_pan_desc_write_words(dgus_ui_t *ui,
                                             const dgus_jpeg_pan_t *pan,
                                             uint16_t offset,
                                             const uint16_t *words,
                                             uint8_t word_count);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_JPEG_PAN_H */
