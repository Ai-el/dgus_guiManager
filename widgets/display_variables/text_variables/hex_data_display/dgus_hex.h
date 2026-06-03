#ifndef DGUS_HEX_H
#define DGUS_HEX_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* HEX data display (14.BIN type 0x13). The public API keeps descriptor writes
 * typed to this widget because manual revisions expose different layouts.
 *
 * Parameters:
 * - vp stores the raw BCD/HEX bytes displayed by the screen.
 * - sp is the descriptor base for type 0x13, or DGUS_SP_NONE.
 * - byte_count is the VP byte span reserved for this widget.
 * - write_words() is useful when application data is already packed as DGUS
 *   big-endian words; write_bytes() sends the byte stream as-is.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
} dgus_hex_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_hex_t, dgus_hex);

/* Create a HEX data display object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS VP for raw HEX/BCD data.
 * sp: descriptor pointer for 14.BIN type 0x13, or DGUS_SP_NONE.
 * byte_count: number of VP bytes reserved for routing/read length.
 */
dgus_hex_t *dgus_hex_create(dgus_page_t *page,
                            uint16_t id,
                            uint16_t vp,
                            uint16_t sp,
                            uint8_t byte_count);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_hex_has_sp(const dgus_hex_t *hex);

/* Write raw BCD/HEX bytes to the object's VP. */
dgus_status_t dgus_hex_write_bytes(dgus_ui_t *ui,
                                   const dgus_hex_t *hex,
                                   const uint8_t *data,
                                   uint16_t byte_len);

/* Write raw BCD/HEX words to the object's VP. */
dgus_status_t dgus_hex_write_words(dgus_ui_t *ui,
                                   const dgus_hex_t *hex,
                                   const uint16_t *words,
                                   uint8_t word_count);

/* Write an arbitrary descriptor word for HEX data display. */
dgus_status_t dgus_hex_desc_write_u16(dgus_ui_t *ui,
                                      const dgus_hex_t *hex,
                                      uint16_t offset,
                                      uint16_t value);

/* Write arbitrary descriptor words for HEX data display. */
dgus_status_t dgus_hex_desc_write_words(dgus_ui_t *ui,
                                        const dgus_hex_t *hex,
                                        uint16_t offset,
                                        const uint16_t *words,
                                        uint8_t word_count);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_HEX_H */
