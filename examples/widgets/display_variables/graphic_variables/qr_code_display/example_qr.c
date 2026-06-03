#include "example_widgets.h"

#define EXAMPLE_QR_ID 27u
#define EXAMPLE_QR_VP 0x1330u
#define EXAMPLE_QR_SP 0x8460u

dgus_qr_t *dgus_example_qr_create(dgus_example_ctx_t *ctx)
{
    dgus_qr_t *qr;

    if (ctx == NULL) {
        return NULL;
    }

    qr = dgus_qr_create(&ctx->page,
                        EXAMPLE_QR_ID,
                        EXAMPLE_QR_VP,
                        EXAMPLE_QR_SP,
                        64u);
    if (qr == NULL) {
        return NULL;
    }

    (void)dgus_qr_set_pos(&ctx->ui, qr, 720, 300);
    (void)dgus_qr_set_unit_pixels(&ctx->ui, qr, 4u);
    (void)dgus_qr_set_fixed_73(&ctx->ui, qr, true);
    (void)dgus_qr_set_text_ascii(&ctx->ui, qr, "https://example.local");

    return qr;
}
