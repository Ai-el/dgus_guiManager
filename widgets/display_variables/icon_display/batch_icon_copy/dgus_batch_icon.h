#ifndef DGUS_BATCH_ICON_H
#define DGUS_BATCH_ICON_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Offsets for batch icon copy display (14.BIN type 0x09). */
#define DGUS_BATCH_ICON_SP_DATA_VP       0x00u
#define DGUS_BATCH_ICON_SP_POS_VP        0x01u
#define DGUS_BATCH_ICON_SP_COPY_DESC_VP  0x02u
#define DGUS_BATCH_ICON_SP_DATA_COUNT    0x03u
#define DGUS_BATCH_ICON_SP_SOURCE_LIB    0x04u
#define DGUS_BATCH_ICON_SP_BACKGROUND_ID 0x05u

/* One batch icon data item. It occupies two VP words in DGUS memory. */
typedef struct {
    uint8_t valid_marker;
    uint8_t icon_id;
    uint8_t mode_filter;
    uint8_t brightness;
} dgus_batch_icon_item_t;

/* Batch data icon copy/paste display.
 *
 * Parameters:
 * - vp points to the batch item data block.
 * - item_count is the number of two-word item records reserved at vp.
 * - data_vp/position_vp/copy_desc_vp are descriptor-linked VP blocks used by
 *   the DGUS batch-copy function.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
} dgus_batch_icon_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_batch_icon_t, dgus_batch_icon);

/* Create a batch icon object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS VP base for item data.
 * sp: descriptor pointer for 14.BIN type 0x09, or DGUS_SP_NONE.
 * item_count: number of batch items; each item occupies two VP words.
 */
dgus_batch_icon_t *dgus_batch_icon_create(dgus_page_t *page,
                                          uint16_t id,
                                          uint16_t vp,
                                          uint16_t sp,
                                          uint16_t item_count);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_batch_icon_has_sp(const dgus_batch_icon_t *icon);

/* Write one batch icon data item to VP + item_index * 2. */
dgus_status_t dgus_batch_icon_set_item(dgus_ui_t *ui,
                                       const dgus_batch_icon_t *icon,
                                       uint16_t item_index,
                                       dgus_batch_icon_item_t item);

/* Write arbitrary descriptor fields for batch icon display. */
dgus_status_t dgus_batch_icon_desc_write_u16(dgus_ui_t *ui,
                                             const dgus_batch_icon_t *icon,
                                             uint16_t offset,
                                             uint16_t value);

/* Configure linked VP ranges used by the batch icon descriptor. */
dgus_status_t dgus_batch_icon_set_sources(dgus_ui_t *ui,
                                          const dgus_batch_icon_t *icon,
                                          uint16_t data_vp,
                                          uint16_t position_vp,
                                          uint16_t copy_desc_vp);

/* Set the number of data items to process. */
dgus_status_t dgus_batch_icon_set_count(dgus_ui_t *ui,
                                        const dgus_batch_icon_t *icon,
                                        uint16_t item_count);

/* Configure optional icon-background copy source. */
dgus_status_t dgus_batch_icon_set_background(dgus_ui_t *ui,
                                             const dgus_batch_icon_t *icon,
                                             uint8_t icon_source,
                                             uint8_t icon_lib,
                                             uint16_t icon_id);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_BATCH_ICON_H */
