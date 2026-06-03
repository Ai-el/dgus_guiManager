#include "example_widgets.h"

#define EXAMPLE_JPEG_ID 11u
#define EXAMPLE_JPEG_VP 0x1170u
#define EXAMPLE_JPEG_SP 0x8100u

dgus_jpeg_t *dgus_example_jpeg_create(dgus_example_ctx_t *ctx)
{
    dgus_jpeg_t *jpeg;
    const uint16_t data_words[] = { 0xFFD8u, 0xFFE0u, 0x0010u, 0x4A46u };

    if (ctx == NULL) {
        return NULL;
    }

    jpeg = dgus_jpeg_create(&ctx->page,
                            EXAMPLE_JPEG_ID,
                            EXAMPLE_JPEG_VP,
                            EXAMPLE_JPEG_SP);
    if (jpeg == NULL) {
        return NULL;
    }

    (void)dgus_jpeg_set_pos(&ctx->ui, jpeg, 200, 40);
    (void)dgus_jpeg_set_window(&ctx->ui, jpeg, 96u, 64u);
    (void)dgus_jpeg_set_brightness_mode(&ctx->ui,
                                        jpeg,
                                        0x80u,
                                        DGUS_JPEG_OPAQUE);
    (void)dgus_jpeg_set_filter_page(&ctx->ui, jpeg, 0u, 0u);
    (void)dgus_jpeg_set_buffer_words(&ctx->ui, jpeg, 4u);
    (void)dgus_jpeg_write_data_words(&ctx->ui, jpeg, data_words, 4u);
    (void)dgus_jpeg_enable(&ctx->ui, jpeg);

    return jpeg;
}
