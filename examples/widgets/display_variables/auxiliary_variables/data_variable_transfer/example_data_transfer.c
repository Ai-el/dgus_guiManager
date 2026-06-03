#include "example_widgets.h"

#define EXAMPLE_DATA_TRANSFER_ID 29u
#define EXAMPLE_DATA_TRANSFER_VP 0x1400u
#define EXAMPLE_DATA_TRANSFER_SP 0x8500u

dgus_data_transfer_t *dgus_example_data_transfer_create(dgus_example_ctx_t *ctx)
{
    dgus_data_transfer_t *transfer;
    const uint8_t runtime_bytes[] = { 0xDEu, 0xADu, 0xBEu, 0xEFu };
    uint8_t predefined[DGUS_DATA_TRANSFER_DATA_BYTES] = {
        0x01u, 0x02u, 0x03u, 0x04u
    };

    if (ctx == NULL) {
        return NULL;
    }

    transfer = dgus_data_transfer_create(&ctx->page,
                                         EXAMPLE_DATA_TRANSFER_ID,
                                         EXAMPLE_DATA_TRANSFER_VP,
                                         EXAMPLE_DATA_TRANSFER_SP);
    if (transfer == NULL) {
        return NULL;
    }

    (void)dgus_data_transfer_set_target_vp(&ctx->ui,
                                           transfer,
                                           EXAMPLE_DATA_TRANSFER_VP);
    (void)dgus_data_transfer_set_auto_upload(&ctx->ui, transfer, true);
    (void)dgus_data_transfer_set_predefined(&ctx->ui,
                                            transfer,
                                            predefined,
                                            sizeof(predefined));
    (void)dgus_data_transfer_write_bytes(&ctx->ui,
                                         transfer,
                                         runtime_bytes,
                                         sizeof(runtime_bytes));

    return transfer;
}
