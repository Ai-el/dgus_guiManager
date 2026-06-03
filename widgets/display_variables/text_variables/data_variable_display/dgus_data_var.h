#ifndef DGUS_DATA_VAR_H
#define DGUS_DATA_VAR_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Offsets for data variable display (14.BIN type 0x10). */
#define DGUS_DATA_VAR_SP_VP          0x00u
#define DGUS_DATA_VAR_SP_POS         0x01u
#define DGUS_DATA_VAR_SP_COLOR       0x03u
#define DGUS_DATA_VAR_SP_FONT        0x04u
#define DGUS_DATA_VAR_SP_INT_DIGITS  0x05u
#define DGUS_DATA_VAR_SP_DEC_MODE    0x06u

/* Numeric data-variable display. SP operations are specific to type 0x10.
 *
 * Parameters:
 * - vp stores the numeric source data.
 * - sp is the descriptor base for type 0x10, or DGUS_SP_NONE.
 * - min_value/max_value are MCU-side metadata for application checks; the DGUS
 *   descriptor range is configured by the PC project or raw descriptor writes.
 * - rgb565 is the text color, font_id/dot_size select a DGUS font, and
 *   decimal_digits/data mode control how VP data is interpreted.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
    int32_t min_value;
    int32_t max_value;
    uint8_t decimals;
} dgus_data_var_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_data_var_t, dgus_data_var);

typedef dgus_data_var_t dgus_value_t;
DGUS_WIDGET_DECLARE_CALLBACKS(dgus_value_t, dgus_value);

/* Create a data-variable display object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS VP storing the numeric value.
 * sp: descriptor pointer for 14.BIN type 0x10, or DGUS_SP_NONE.
 */
dgus_data_var_t *dgus_data_var_create(dgus_page_t *page,
                                      uint16_t id,
                                      uint16_t vp,
                                      uint16_t sp);

/* Backward-compatible alias for a numeric value display. */
dgus_value_t *dgus_value_create(dgus_page_t *page,
                                uint16_t id,
                                uint16_t vp,
                                uint16_t sp);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_data_var_has_sp(const dgus_data_var_t *data);

/* Set numeric range metadata stored on the MCU side.
 * min_value/max_value: application-level limits; no DGUS frame is sent.
 */
void dgus_data_var_set_range(dgus_data_var_t *data,
                             int32_t min_value,
                             int32_t max_value);
void dgus_value_set_range(dgus_value_t *value,
                          int32_t min_value,
                          int32_t max_value);

/* Write numeric data to the object's VP.
 * value: raw VP word; decimal display depends on descriptor decimal mode.
 */
dgus_status_t dgus_data_var_set_u16(dgus_ui_t *ui,
                                    const dgus_data_var_t *data,
                                    uint16_t value);
dgus_status_t dgus_value_set_u16(dgus_ui_t *ui,
                                 const dgus_value_t *value,
                                 uint16_t data);

/* Write arbitrary descriptor fields for data variable display. */
dgus_status_t dgus_data_var_desc_write_u16(dgus_ui_t *ui,
                                           const dgus_data_var_t *data,
                                           uint16_t offset,
                                           uint16_t value);
dgus_status_t dgus_data_var_desc_write_words(dgus_ui_t *ui,
                                             const dgus_data_var_t *data,
                                             uint16_t offset,
                                             const uint16_t *words,
                                             uint8_t word_count);

/* Change the linked VP at SP+0. 0xFF00 hides the display per the manual. */
dgus_status_t dgus_data_var_set_desc_vp(dgus_ui_t *ui,
                                        const dgus_data_var_t *data,
                                        uint16_t vp);

/* Hide by writing 0xFF00 to SP+0. */
dgus_status_t dgus_data_var_hide(dgus_ui_t *ui,
                                 const dgus_data_var_t *data);

/* Show by restoring a linked VP at SP+0. */
dgus_status_t dgus_data_var_show(dgus_ui_t *ui,
                                 const dgus_data_var_t *data);

/* Move the display by writing X,Y to SP+1. */
dgus_status_t dgus_data_var_set_pos(dgus_ui_t *ui,
                                    const dgus_data_var_t *data,
                                    int16_t x,
                                    int16_t y);

/* Change the display color at SP+3. */
dgus_status_t dgus_data_var_set_color(dgus_ui_t *ui,
                                      const dgus_data_var_t *data,
                                      uint16_t rgb565);

/* Set font id and dot size at SP+4. */
dgus_status_t dgus_data_var_set_font(dgus_ui_t *ui,
                                     const dgus_data_var_t *data,
                                     uint8_t font_id,
                                     uint8_t dot_size);

/* Set alignment and integer digit count at SP+5. */
dgus_status_t dgus_data_var_set_integer_digits(dgus_ui_t *ui,
                                               const dgus_data_var_t *data,
                                               dgus_align_t align,
                                               uint8_t digits);

/* Set decimal digit count and data mode at SP+6.
 * decimal_digits: digits after the decimal point.
 * mode: one of dgus_data_mode_t, matching the VP data format.
 */
dgus_status_t dgus_data_var_set_decimal_mode(dgus_ui_t *ui,
                                             const dgus_data_var_t *data,
                                             uint8_t decimal_digits,
                                             dgus_data_mode_t mode);



#ifdef __cplusplus
}
#endif

#endif /* DGUS_DATA_VAR_H */
