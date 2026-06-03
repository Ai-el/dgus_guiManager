#include "dgus_data_transfer.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_data_transfer_t, dgus_data_transfer)
#include <string.h>

dgus_data_transfer_t *dgus_data_transfer_create(dgus_page_t *page,
                                                uint16_t id,
                                                uint16_t vp,
                                                uint16_t sp)
{
    dgus_data_transfer_t *transfer;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    transfer = (dgus_data_transfer_t *)dgus_page_alloc(page, sizeof(*transfer));
    if (transfer == NULL) {
        return NULL;
    }
    dgus_widget_init_obj(&transfer->obj, id, DGUS_OBJ_DATA_TRANSFER, vp, 11u);
    transfer->sp = sp;
    if (dgus_page_add_obj(page, &transfer->obj) != DGUS_OK) {
        return NULL;
    }

    return transfer;
}

bool dgus_data_transfer_has_sp(const dgus_data_transfer_t *transfer)
{
    return (transfer != NULL) && dgus_widget_has_sp(transfer->sp);
}

dgus_status_t dgus_data_transfer_write_bytes(dgus_ui_t *ui,
                                             const dgus_data_transfer_t *transfer,
                                             const uint8_t *data,
                                             uint16_t byte_len)
{
    if (transfer == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_write_bytes(ui, &transfer->obj, data, byte_len);
}

dgus_status_t dgus_data_transfer_desc_write_u16(dgus_ui_t *ui,
                                                const dgus_data_transfer_t *transfer,
                                                uint16_t offset,
                                                uint16_t value)
{
    if (!dgus_data_transfer_has_sp(transfer)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &transfer->obj, transfer->sp, offset, value);
}

dgus_status_t dgus_data_transfer_set_target_vp(dgus_ui_t *ui,
                                               const dgus_data_transfer_t *transfer,
                                               uint16_t vp)
{
    return dgus_data_transfer_desc_write_u16(ui,
                                             transfer,
                                             DGUS_DATA_TRANSFER_SP_VP,
                                             vp);
}

dgus_status_t dgus_data_transfer_set_auto_upload(dgus_ui_t *ui,
                                                 const dgus_data_transfer_t *transfer,
                                                 bool enabled)
{
    return dgus_data_transfer_desc_write_u16(ui,
                                             transfer,
                                             DGUS_DATA_TRANSFER_SP_AUTO_UPLOAD,
                                             dgus_pack_u8(enabled ? 0xFFu : 0u, 0u));
}

dgus_status_t dgus_data_transfer_set_predefined(dgus_ui_t *ui,
                                                const dgus_data_transfer_t *transfer,
                                                const uint8_t *data,
                                                uint8_t byte_len)
{
    uint8_t payload[DGUS_DATA_TRANSFER_DATA_BYTES];

    if (!dgus_data_transfer_has_sp(transfer) ||
        ((data == NULL) && (byte_len > 0u)) ||
        (byte_len > DGUS_DATA_TRANSFER_DATA_BYTES)) {
        return DGUS_ERR_ARG;
    }

    memset(payload, 0, sizeof(payload));
    if (byte_len > 0u) {
        memcpy(payload, data, byte_len);
    }

    return dgus_obj_desc_write_bytes_at(ui, &transfer->obj, transfer->sp,
                                 DGUS_DATA_TRANSFER_SP_DATA,
                                 payload,
                                 sizeof(payload));
}
