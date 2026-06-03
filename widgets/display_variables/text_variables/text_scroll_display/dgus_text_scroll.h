#ifndef DGUS_TEXT_SCROLL_H
#define DGUS_TEXT_SCROLL_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Offsets for text scrolling display (14.BIN type 0x14). */
#define DGUS_TEXT_SCROLL_SP_VP              0x00u
#define DGUS_TEXT_SCROLL_SP_ROLLING         0x01u
#define DGUS_TEXT_SCROLL_SP_ADJUST          0x02u
#define DGUS_TEXT_SCROLL_SP_COLOR           0x03u
#define DGUS_TEXT_SCROLL_SP_BOX             0x04u
#define DGUS_TEXT_SCROLL_SP_FONT_ID         0x08u
#define DGUS_TEXT_SCROLL_SP_FONT_DOTS       0x09u
#define DGUS_TEXT_SCROLL_SP_ENCODE_SPACING  0x0Au
#define DGUS_TEXT_SCROLL_VP_TEXT_OFFSET     0x03u

typedef enum {
    DGUS_TEXT_SCROLL_MODE_LEFT = 0,
    DGUS_TEXT_SCROLL_MODE_RIGHT = 1,
    DGUS_TEXT_SCROLL_MODE_UP = 2,
    DGUS_TEXT_SCROLL_MODE_DOWN = 3
} dgus_text_scroll_mode_t;

typedef enum {
    DGUS_TEXT_SCROLL_ALIGN_LEFT = 0,
    DGUS_TEXT_SCROLL_ALIGN_RIGHT = 1,
    DGUS_TEXT_SCROLL_ALIGN_CENTER = 2
} dgus_text_scroll_align_t;

/* Text scrolling display. Text content starts at VP+3 per the manual.
 *
 * Parameters:
 * - vp is the control block base; text bytes start at VP+3.
 * - sp is the descriptor base for type 0x14, or DGUS_SP_NONE.
 * - mode selects left/right/up/down scrolling, distance is pixels per cycle.
 * - align selects text alignment inside the scroll box.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
    uint8_t max_chars;
    bool terminate;
} dgus_text_scroll_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_text_scroll_t, dgus_text_scroll);

/* Create a text scrolling display object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS scroll-control VP base.
 * sp: descriptor pointer for 14.BIN type 0x14, or DGUS_SP_NONE.
 * max_chars: maximum ASCII characters written to VP+3.
 */
dgus_text_scroll_t *dgus_text_scroll_create(dgus_page_t *page,
                                            uint16_t id,
                                            uint16_t vp,
                                            uint16_t sp,
                                            uint8_t max_chars);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_text_scroll_has_sp(const dgus_text_scroll_t *scroll);

/* Configure whether text writes append the DGUS 0xFFFF terminator. */
void dgus_text_scroll_set_terminate(dgus_text_scroll_t *scroll, bool terminate);

/* Write ASCII text to VP+3, preserving the manual's three reserved VP words. */
dgus_status_t dgus_text_scroll_set_text_ascii(dgus_ui_t *ui,
                                              const dgus_text_scroll_t *scroll,
                                              const char *text);

/* Write an arbitrary descriptor word for text scrolling display. */
dgus_status_t dgus_text_scroll_desc_write_u16(dgus_ui_t *ui,
                                              const dgus_text_scroll_t *scroll,
                                              uint16_t offset,
                                              uint16_t value);

/* Change the linked VP at SP+0. 0xFF00 hides the display. */
dgus_status_t dgus_text_scroll_set_desc_vp(dgus_ui_t *ui,
                                           const dgus_text_scroll_t *scroll,
                                           uint16_t vp);

/* Hide by writing 0xFF00 to SP+0. */
dgus_status_t dgus_text_scroll_hide(dgus_ui_t *ui,
                                    const dgus_text_scroll_t *scroll);

/* Show by restoring a linked VP at SP+0. */
dgus_status_t dgus_text_scroll_show(dgus_ui_t *ui,
                                    const dgus_text_scroll_t *scroll);

/* Configure scroll mode and per-cycle pixel distance at SP+1.
 * mode: one of dgus_text_scroll_mode_t.
 * distance: pixels moved on each DGUS scroll step.
 */
dgus_status_t dgus_text_scroll_set_rolling(dgus_ui_t *ui,
                                           const dgus_text_scroll_t *scroll,
                                           dgus_text_scroll_mode_t mode,
                                           uint8_t distance);

/* Configure text alignment at SP+2. */
dgus_status_t dgus_text_scroll_set_align(dgus_ui_t *ui,
                                         const dgus_text_scroll_t *scroll,
                                         dgus_text_scroll_align_t align);

/* Change the text color at SP+3. */
dgus_status_t dgus_text_scroll_set_color(dgus_ui_t *ui,
                                         const dgus_text_scroll_t *scroll,
                                         uint16_t rgb565);

/* Change the scroll box at SP+4. */
dgus_status_t dgus_text_scroll_set_box(dgus_ui_t *ui,
                                       const dgus_text_scroll_t *scroll,
                                       dgus_rect_t box);

/* Change ASCII/non-ASCII font ids at SP+8. */
dgus_status_t dgus_text_scroll_set_font_ids(dgus_ui_t *ui,
                                            const dgus_text_scroll_t *scroll,
                                            uint8_t ascii_font,
                                            uint8_t non_ascii_font);

/* Change font dot dimensions at SP+9. */
dgus_status_t dgus_text_scroll_set_font_dots(dgus_ui_t *ui,
                                             const dgus_text_scroll_t *scroll,
                                             uint8_t x_dots,
                                             uint8_t y_dots);

/* Configure encode mode and horizontal spacing at SP+0x0A. */
dgus_status_t dgus_text_scroll_set_encode_spacing(dgus_ui_t *ui,
                                                  const dgus_text_scroll_t *scroll,
                                                  uint8_t encode_mode,
                                                  uint8_t horizontal_spacing);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_TEXT_SCROLL_H */
