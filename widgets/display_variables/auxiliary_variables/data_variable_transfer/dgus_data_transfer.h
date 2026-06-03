#ifndef DGUS_DATA_TRANSFER_H
#define DGUS_DATA_TRANSFER_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Offsets for data variable transfer (14.BIN type 0x30). */
#define DGUS_DATA_TRANSFER_SP_VP          0x00u
#define DGUS_DATA_TRANSFER_SP_AUTO_UPLOAD 0x01u
#define DGUS_DATA_TRANSFER_SP_DATA        0x06u
#define DGUS_DATA_TRANSFER_DATA_BYTES     22u

/* Data-transfer helper. The screen copies predefined data after page entry.
 *
 * Parameters:
 * - vp is the target VP for runtime writes and transferred data.
 * - sp is the descriptor base for type 0x30, or DGUS_SP_NONE.
 * - predefined data is the 22-byte payload stored at SP+6.
 * - auto upload controls whether DGUS sends the data over UART when the page
 *   first becomes valid.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
} dgus_data_transfer_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_data_transfer_t, dgus_data_transfer);

/* Create a data-transfer object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS target VP for transfer data.
 * sp: descriptor pointer for 14.BIN type 0x30, or DGUS_SP_NONE.
 */
dgus_data_transfer_t *dgus_data_transfer_create(dgus_page_t *page,
                                                uint16_t id,
                                                uint16_t vp,
                                                uint16_t sp);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_data_transfer_has_sp(const dgus_data_transfer_t *transfer);

/* Write arbitrary bytes to the transfer target VP. */
dgus_status_t dgus_data_transfer_write_bytes(dgus_ui_t *ui,
                                             const dgus_data_transfer_t *transfer,
                                             const uint8_t *data,
                                             uint16_t byte_len);

/* Write an arbitrary descriptor word for data-transfer display. */
dgus_status_t dgus_data_transfer_desc_write_u16(dgus_ui_t *ui,
                                                const dgus_data_transfer_t *transfer,
                                                uint16_t offset,
                                                uint16_t value);

/* Change the target VP at SP+0. */
dgus_status_t dgus_data_transfer_set_target_vp(dgus_ui_t *ui,
                                               const dgus_data_transfer_t *transfer,
                                               uint16_t vp);

/* Enable or disable automatic serial upload when the page first becomes valid. */
dgus_status_t dgus_data_transfer_set_auto_upload(dgus_ui_t *ui,
                                                 const dgus_data_transfer_t *transfer,
                                                 bool enabled);

/* Set the predefined 22-byte payload at SP+6.
 * data: bytes copied into the descriptor payload field.
 * byte_len: 0..DGUS_DATA_TRANSFER_DATA_BYTES; shorter data is padded by DGUS
 *   project defaults/previous content depending on screen behavior.
 */
dgus_status_t dgus_data_transfer_set_predefined(dgus_ui_t *ui,
                                                const dgus_data_transfer_t *transfer,
                                                const uint8_t *data,
                                                uint8_t byte_len);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_DATA_TRANSFER_H */
