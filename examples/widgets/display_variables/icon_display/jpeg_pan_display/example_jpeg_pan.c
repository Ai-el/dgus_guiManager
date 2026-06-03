#include "example_widgets.h"

#define EXAMPLE_JPEG_PAN_ID 10u
#define EXAMPLE_JPEG_PAN_VP 0x1160u
#define EXAMPLE_JPEG_PAN_SP 0x80E0u

dgus_jpeg_pan_t *dgus_example_jpeg_pan_create(dgus_example_ctx_t *ctx)
{
    dgus_jpeg_pan_t *pan;
    const uint16_t control_words[] = { 0x0001u, 0x0020u, 0x0010u, 0x0000u };
    const uint16_t desc_words[] = { 0x1160u, 0x0001u };

    if (ctx == NULL) {
        return NULL;
    }

    pan = dgus_jpeg_pan_create(&ctx->page,
                               EXAMPLE_JPEG_PAN_ID,
                               EXAMPLE_JPEG_PAN_VP,
                               EXAMPLE_JPEG_PAN_SP,
                               4u);
    if (pan == NULL) {
        return NULL;
    }

    (void)dgus_jpeg_pan_desc_write_words(&ctx->ui, pan, 0u, desc_words, 2u);
    (void)dgus_jpeg_pan_write_words(&ctx->ui, pan, control_words, 4u);

    return pan;
}
