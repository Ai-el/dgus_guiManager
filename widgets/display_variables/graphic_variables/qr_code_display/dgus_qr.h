#ifndef DGUS_QR_H
#define DGUS_QR_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Offsets for QR display (14.BIN type 0x25). */
#define DGUS_QR_SP_VP            0x00u
#define DGUS_QR_SP_POS           0x01u
#define DGUS_QR_SP_UNIT_PIXELS   0x03u
#define DGUS_QR_SP_FIX_MODE      0x05u

/* QR display.
 *
 * Parameters:
 * - vp stores QR text bytes and terminator.
 * - sp is the descriptor base for type 0x25, or DGUS_SP_NONE.
 * - max_bytes limits QR content length.
 * - unit_pixels controls QR module size; fixed enables the manual's 73x73 mode.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
    uint16_t max_bytes;
} dgus_qr_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_qr_t, dgus_qr);

/* Create a QR display object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS VP storing QR content.
 * sp: descriptor pointer for 14.BIN type 0x25, or DGUS_SP_NONE.
 * max_bytes: maximum content bytes written by dgus_qr_set_text_ascii().
 */
dgus_qr_t *dgus_qr_create(dgus_page_t *page,
                          uint16_t id,
                          uint16_t vp,
                          uint16_t sp,
                          uint16_t max_bytes);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_qr_has_sp(const dgus_qr_t *qr);

/* Write QR content as ASCII bytes and append 0xFFFF terminator. */
dgus_status_t dgus_qr_set_text_ascii(dgus_ui_t *ui,
                                     const dgus_qr_t *qr,
                                     const char *text);

/* Write an arbitrary descriptor word for QR display. */
dgus_status_t dgus_qr_desc_write_u16(dgus_ui_t *ui,
                                     const dgus_qr_t *qr,
                                     uint16_t offset,
                                     uint16_t value);

/* Move the QR code by writing X,Y to SP+1. */
dgus_status_t dgus_qr_set_pos(dgus_ui_t *ui,
                              const dgus_qr_t *qr,
                              int16_t x,
                              int16_t y);

/* Configure physical pixels per QR unit. */
dgus_status_t dgus_qr_set_unit_pixels(dgus_ui_t *ui,
                                      const dgus_qr_t *qr,
                                      uint16_t unit_pixels);

/* Configure fixed 73x73 mode at SP+5. */
dgus_status_t dgus_qr_set_fixed_73(dgus_ui_t *ui,
                                   const dgus_qr_t *qr,
                                   bool fixed);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_QR_H */
