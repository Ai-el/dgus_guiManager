#ifndef DGUS_DGUSII_TEXT_H
#define DGUS_DGUSII_TEXT_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Offsets for DGUSII anti-aliased text display (14.BIN type 0x16). */
#define DGUS_DGUSII_TEXT_SP_VP              0x00u
#define DGUS_DGUSII_TEXT_SP_POS             0x01u
#define DGUS_DGUSII_TEXT_SP_COLOR           0x03u
#define DGUS_DGUSII_TEXT_SP_BOX             0x05u
#define DGUS_DGUSII_TEXT_SP_LENGTH          0x09u
#define DGUS_DGUSII_TEXT_SP_LIB_ID          0x0Au
#define DGUS_DGUSII_TEXT_SP_MODE_HOR        0x0Bu
#define DGUS_DGUSII_TEXT_SP_VER_RESERVED    0x0Cu

/* DGUSII anti-aliased text display using grayscale font libraries.
 *
 * Parameters:
 * - vp stores encoded text bytes.
 * - sp is the descriptor base for type 0x16, or DGUS_SP_NONE.
 * - max_bytes limits raw encoded writes.
 * - lib_id selects the grayscale font library.
 * - display_mode/horizontal_spacing/vertical_spacing map directly to the
 *   DGUSII text descriptor fields.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
    uint16_t max_bytes;
    bool terminate;
} dgus_dgusii_text_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_dgusii_text_t, dgus_dgusii_text);

/* Create a DGUSII text display object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS text VP base.
 * sp: descriptor pointer for 14.BIN type 0x16, or DGUS_SP_NONE.
 * max_bytes: maximum encoded byte count written to VP.
 */
dgus_dgusii_text_t *dgus_dgusii_text_create(dgus_page_t *page,
                                            uint16_t id,
                                            uint16_t vp,
                                            uint16_t sp,
                                            uint16_t max_bytes);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_dgusii_text_has_sp(const dgus_dgusii_text_t *text);

/* Configure whether text writes append the DGUS 0xFFFF terminator. */
void dgus_dgusii_text_set_terminate(dgus_dgusii_text_t *text, bool terminate);

/* Write raw encoded text bytes to the object's VP. */
dgus_status_t dgus_dgusii_text_write_bytes(dgus_ui_t *ui,
                                           const dgus_dgusii_text_t *text,
                                           const uint8_t *bytes,
                                           uint16_t byte_len);

/* Write ASCII text as DGUS 16-bit text data. */
dgus_status_t dgus_dgusii_text_set_text_ascii(dgus_ui_t *ui,
                                              const dgus_dgusii_text_t *text,
                                              const char *value);

/* Write an arbitrary descriptor word for DGUSII text display. */
dgus_status_t dgus_dgusii_text_desc_write_u16(dgus_ui_t *ui,
                                              const dgus_dgusii_text_t *text,
                                              uint16_t offset,
                                              uint16_t value);

/* Change the linked VP at SP+0. 0xFF00 hides the display. */
dgus_status_t dgus_dgusii_text_set_desc_vp(dgus_ui_t *ui,
                                           const dgus_dgusii_text_t *text,
                                           uint16_t vp);

/* Hide by writing 0xFF00 to SP+0. */
dgus_status_t dgus_dgusii_text_hide(dgus_ui_t *ui,
                                    const dgus_dgusii_text_t *text);

/* Show by restoring a linked VP at SP+0. */
dgus_status_t dgus_dgusii_text_show(dgus_ui_t *ui,
                                    const dgus_dgusii_text_t *text);

/* Move the display by writing X,Y to SP+1. */
dgus_status_t dgus_dgusii_text_set_pos(dgus_ui_t *ui,
                                       const dgus_dgusii_text_t *text,
                                       int16_t x,
                                       int16_t y);

/* Change the text color at SP+3. */
dgus_status_t dgus_dgusii_text_set_color(dgus_ui_t *ui,
                                         const dgus_dgusii_text_t *text,
                                         uint16_t rgb565);

/* Change text display box at SP+5. */
dgus_status_t dgus_dgusii_text_set_box(dgus_ui_t *ui,
                                       const dgus_dgusii_text_t *text,
                                       dgus_rect_t box);

/* Change displayed byte length at SP+9. */
dgus_status_t dgus_dgusii_text_set_length(dgus_ui_t *ui,
                                          const dgus_dgusii_text_t *text,
                                          uint16_t byte_len);

/* Set grayscale font library id at SP+0x0A. */
dgus_status_t dgus_dgusii_text_set_lib_id(dgus_ui_t *ui,
                                          const dgus_dgusii_text_t *text,
                                          uint16_t lib_id);

/* Configure display mode and horizontal spacing at SP+0x0B. */
dgus_status_t dgus_dgusii_text_set_mode_spacing(dgus_ui_t *ui,
                                                const dgus_dgusii_text_t *text,
                                                uint8_t display_mode,
                                                uint8_t horizontal_spacing);

/* Configure vertical spacing at SP+0x0C high byte. */
dgus_status_t dgus_dgusii_text_set_vertical_spacing(dgus_ui_t *ui,
                                                    const dgus_dgusii_text_t *text,
                                                    uint8_t vertical_spacing);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_DGUSII_TEXT_H */
