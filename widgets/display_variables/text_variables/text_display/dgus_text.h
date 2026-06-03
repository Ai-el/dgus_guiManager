#ifndef DGUS_TEXT_H
#define DGUS_TEXT_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Offsets for text display (14.BIN type 0x11). */
#define DGUS_TEXT_SP_VP             0x00u
#define DGUS_TEXT_SP_POS            0x01u
#define DGUS_TEXT_SP_COLOR          0x03u
#define DGUS_TEXT_SP_BOX            0x04u
#define DGUS_TEXT_SP_LENGTH         0x08u
#define DGUS_TEXT_SP_FONT_ID        0x09u
#define DGUS_TEXT_SP_FONT_DOTS      0x0Au
#define DGUS_TEXT_SP_ENCODE_SPACING 0x0Bu
#define DGUS_TEXT_SP_LINE_SPACING   0x0Cu

/* Text display object. SP operations are specific to type 0x11.
 *
 * Parameters:
 * - vp stores text data. ASCII helpers write one DGUS word per character.
 * - sp is the descriptor base for type 0x11, or DGUS_SP_NONE.
 * - max_chars limits ASCII writes so they fit the VP block.
 * - terminate controls whether ASCII writes append the DGUS 0xFFFF terminator.
 * - byte_len is the displayed byte count field in the descriptor.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
    uint8_t max_chars;
    bool terminate;
} dgus_text_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_text_t, dgus_text);

typedef dgus_text_t dgus_label_t;
DGUS_WIDGET_DECLARE_CALLBACKS(dgus_label_t, dgus_label);

/* Create a text display object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS text VP base.
 * sp: descriptor pointer for 14.BIN type 0x11, or DGUS_SP_NONE.
 * max_chars: maximum ASCII characters written by dgus_text_set_text_ascii().
 */
dgus_text_t *dgus_text_create(dgus_page_t *page,
                              uint16_t id,
                              uint16_t vp,
                              uint16_t sp,
                              uint8_t max_chars);

/* Backward-compatible alias for text display. */
dgus_label_t *dgus_label_create(dgus_page_t *page,
                                uint16_t id,
                                uint16_t vp,
                                uint16_t sp,
                                uint8_t max_chars);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_text_has_sp(const dgus_text_t *text);

/* Configure whether text writes append the DGUS 0xFFFF terminator.
 * terminate: true appends 0xFFFF after the last character.
 */
void dgus_text_set_terminate(dgus_text_t *text, bool terminate);

/* Write ASCII text using a text object's configured maximum length.
 * value: NUL-terminated ASCII string; characters past max_chars are ignored.
 */
dgus_status_t dgus_text_set_text_ascii(dgus_ui_t *ui,
                                       const dgus_text_t *text,
                                       const char *value);
dgus_status_t dgus_label_set_text_ascii(dgus_ui_t *ui,
                                        const dgus_label_t *label,
                                        const char *value);

/* Write arbitrary descriptor fields for text display. */
dgus_status_t dgus_text_desc_write_u16(dgus_ui_t *ui,
                                       const dgus_text_t *text,
                                       uint16_t offset,
                                       uint16_t value);
dgus_status_t dgus_text_desc_write_words(dgus_ui_t *ui,
                                         const dgus_text_t *text,
                                         uint16_t offset,
                                         const uint16_t *words,
                                         uint8_t word_count);

/* Change the linked VP at SP+0. 0xFF00 hides the display per the manual. */
dgus_status_t dgus_text_set_desc_vp(dgus_ui_t *ui,
                                    const dgus_text_t *text,
                                    uint16_t vp);

/* Hide by writing 0xFF00 to SP+0. */
dgus_status_t dgus_text_hide(dgus_ui_t *ui, const dgus_text_t *text);

/* Show by restoring a linked VP at SP+0. */
dgus_status_t dgus_text_show(dgus_ui_t *ui, const dgus_text_t *text);

/* Move the display by writing X,Y to SP+1. */
dgus_status_t dgus_text_set_pos(dgus_ui_t *ui,
                                const dgus_text_t *text,
                                int16_t x,
                                int16_t y);

/* Change the text color at SP+3. */
dgus_status_t dgus_text_set_color(dgus_ui_t *ui,
                                  const dgus_text_t *text,
                                  uint16_t rgb565);

/* Change text display box at SP+4.
 * box: x0,y0,x1,y1 pixel rectangle used for clipping/layout.
 */
dgus_status_t dgus_text_set_box(dgus_ui_t *ui,
                                const dgus_text_t *text,
                                dgus_rect_t box);

/* Change the displayed byte length at SP+8.
 * byte_len: number of bytes the screen should render from the VP text buffer.
 */
dgus_status_t dgus_text_set_length(dgus_ui_t *ui,
                                   const dgus_text_t *text,
                                   uint16_t byte_len);

/* Change ASCII/non-ASCII font ids at SP+9. */
dgus_status_t dgus_text_set_font_ids(dgus_ui_t *ui,
                                     const dgus_text_t *text,
                                     uint8_t ascii_font,
                                     uint8_t non_ascii_font);

/* Change font dot dimensions at SP+0x0A. */
dgus_status_t dgus_text_set_font_dots(dgus_ui_t *ui,
                                      const dgus_text_t *text,
                                      uint8_t x_dots,
                                      uint8_t y_dots);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_TEXT_H */
