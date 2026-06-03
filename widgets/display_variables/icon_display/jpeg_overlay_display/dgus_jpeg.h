#ifndef DGUS_JPEG_H
#define DGUS_JPEG_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Offsets for variable-data JPEG overlay display (14.BIN type 0x08). */
#define DGUS_JPEG_SP_VP           0x00u
#define DGUS_JPEG_SP_POS          0x01u
#define DGUS_JPEG_SP_WIDTH        0x03u
#define DGUS_JPEG_SP_HEIGHT       0x04u
#define DGUS_JPEG_SP_DIM_MODE     0x05u
#define DGUS_JPEG_SP_FILTER_PAGE  0x06u

/* VP offsets for variable-data JPEG overlay display. */
#define DGUS_JPEG_VP_ENABLE       0x00u
#define DGUS_JPEG_VP_WORD_LENGTH  0x01u
#define DGUS_JPEG_VP_DATA         0x02u

typedef enum {
    DGUS_JPEG_TRANSPARENT = 0,
    DGUS_JPEG_OPAQUE = 1
} dgus_jpeg_display_mode_t;

/* Variable-data JPEG overlay display. VP controls image data and visibility.
 *
 * Parameters:
 * - vp is a control block: VP+0 enable marker, VP+1 word length, VP+2 data.
 * - sp is the descriptor base for type 0x08, or DGUS_SP_NONE.
 * - brightness/filter_strength are DGUS descriptor bytes; valid practical
 *   values are normally 0x00..0xFF.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
} dgus_jpeg_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_jpeg_t, dgus_jpeg);

/* Create a JPEG overlay display object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS JPEG control VP block base.
 * sp: descriptor pointer for 14.BIN type 0x08, or DGUS_SP_NONE.
 */
dgus_jpeg_t *dgus_jpeg_create(dgus_page_t *page,
                              uint16_t id,
                              uint16_t vp,
                              uint16_t sp);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_jpeg_has_sp(const dgus_jpeg_t *jpeg);

/* Enable JPEG display by writing 0x5AA5 to VP. */
dgus_status_t dgus_jpeg_enable(dgus_ui_t *ui, const dgus_jpeg_t *jpeg);

/* Disable JPEG display by clearing VP. */
dgus_status_t dgus_jpeg_disable(dgus_ui_t *ui, const dgus_jpeg_t *jpeg);

/* Set JPEG buffer word length at VP+1. */
dgus_status_t dgus_jpeg_set_buffer_words(dgus_ui_t *ui,
                                         const dgus_jpeg_t *jpeg,
                                         uint16_t word_count);

/* Write JPEG buffer words starting at VP+2.
 * words: encoded JPEG data words already packed in DGUS word order.
 * word_count: number of words to send in this frame.
 */
dgus_status_t dgus_jpeg_write_data_words(dgus_ui_t *ui,
                                         const dgus_jpeg_t *jpeg,
                                         const uint16_t *words,
                                         uint8_t word_count);

/* Write an arbitrary descriptor word for JPEG overlay display. */
dgus_status_t dgus_jpeg_desc_write_u16(dgus_ui_t *ui,
                                       const dgus_jpeg_t *jpeg,
                                       uint16_t offset,
                                       uint16_t value);

/* Move the JPEG overlay by writing X,Y to SP+1. */
dgus_status_t dgus_jpeg_set_pos(dgus_ui_t *ui,
                                const dgus_jpeg_t *jpeg,
                                int16_t x,
                                int16_t y);

/* Set the JPEG display window width and height. */
dgus_status_t dgus_jpeg_set_window(dgus_ui_t *ui,
                                   const dgus_jpeg_t *jpeg,
                                   uint16_t width,
                                   uint16_t height);

/* Set brightness and transparent/background display mode. */
dgus_status_t dgus_jpeg_set_brightness_mode(dgus_ui_t *ui,
                                            const dgus_jpeg_t *jpeg,
                                            uint8_t brightness,
                                            dgus_jpeg_display_mode_t mode);

/* Set background filter strength and variable memory page selector. */
dgus_status_t dgus_jpeg_set_filter_page(dgus_ui_t *ui,
                                        const dgus_jpeg_t *jpeg,
                                        uint8_t filter_strength,
                                        uint8_t vp_page);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_JPEG_H */
