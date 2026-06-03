#ifndef DGUS_ART_TEXT_H
#define DGUS_ART_TEXT_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Offsets for art text / icon digit display (14.BIN type 0x03). */
#define DGUS_ART_TEXT_SP_VP          0x00u
#define DGUS_ART_TEXT_SP_POS         0x01u
#define DGUS_ART_TEXT_SP_ICON0       0x03u
#define DGUS_ART_TEXT_SP_LIB_MODE    0x04u
#define DGUS_ART_TEXT_SP_DIGITS      0x05u
#define DGUS_ART_TEXT_SP_DATA_ALIGN  0x06u

/* Art text/icon digit display. Numeric VP data is rendered using digit icons.
 *
 * Parameters:
 * - vp stores the numeric value.
 * - word_count is the number of DGUS words reserved for the value.
 * - icon0 is the icon ID for digit 0; later digits normally follow in order.
 * - integer_digits/decimal_digits define the displayed digit layout.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
} dgus_art_text_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_art_text_t, dgus_art_text);

/* Create an art text/icon digit display object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS VP storing the numeric data.
 * sp: descriptor pointer for 14.BIN type 0x03, or DGUS_SP_NONE.
 * word_count: number of VP words owned by this value.
 */
dgus_art_text_t *dgus_art_text_create(dgus_page_t *page,
                                      uint16_t id,
                                      uint16_t vp,
                                      uint16_t sp,
                                      uint8_t word_count);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_art_text_has_sp(const dgus_art_text_t *art);

/* Write one numeric value to the object's VP.
 * value: numeric value rendered by the icon digits.
 */
dgus_status_t dgus_art_text_set_u16(dgus_ui_t *ui,
                                    const dgus_art_text_t *art,
                                    uint16_t value);

/* Write an arbitrary descriptor word for art text display. */
dgus_status_t dgus_art_text_desc_write_u16(dgus_ui_t *ui,
                                           const dgus_art_text_t *art,
                                           uint16_t offset,
                                           uint16_t value);

/* Move the display by writing X,Y to SP+1. */
dgus_status_t dgus_art_text_set_pos(dgus_ui_t *ui,
                                    const dgus_art_text_t *art,
                                    int16_t x,
                                    int16_t y);

/* Configure the icon ID for digit 0 at SP+3. */
dgus_status_t dgus_art_text_set_icon0(dgus_ui_t *ui,
                                      const dgus_art_text_t *art,
                                      uint16_t icon0);

/* Configure integer and decimal digit counts at SP+5.
 * integer_digits: digits before the decimal point.
 * decimal_digits: digits after the decimal point.
 */
dgus_status_t dgus_art_text_set_digits(dgus_ui_t *ui,
                                       const dgus_art_text_t *art,
                                       uint8_t integer_digits,
                                       uint8_t decimal_digits);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_ART_TEXT_H */
