#include "example_widgets.h"

#define EXAMPLE_DGUSII_TEXT_ID 20u
#define EXAMPLE_DGUSII_TEXT_VP 0x1250u
#define EXAMPLE_DGUSII_TEXT_SP 0x82E0u

dgus_dgusii_text_t *dgus_example_dgusii_text_create(dgus_example_ctx_t *ctx)
{
    dgus_dgusii_text_t *text;
    dgus_rect_t box;

    if (ctx == NULL) {
        return NULL;
    }

    text = dgus_dgusii_text_create(&ctx->page,
                                   EXAMPLE_DGUSII_TEXT_ID,
                                   EXAMPLE_DGUSII_TEXT_VP,
                                   EXAMPLE_DGUSII_TEXT_SP,
                                   32u);
    if (text == NULL) {
        return NULL;
    }

    box.x0 = 320;
    box.y0 = 210;
    box.x1 = 520;
    box.y1 = 250;

    dgus_dgusii_text_set_terminate(text, true);
    (void)dgus_dgusii_text_set_pos(&ctx->ui, text, 320, 210);
    (void)dgus_dgusii_text_set_color(&ctx->ui, text, 0xFFFFu);
    (void)dgus_dgusii_text_set_box(&ctx->ui, text, box);
    (void)dgus_dgusii_text_set_length(&ctx->ui, text, 32u);
    (void)dgus_dgusii_text_set_lib_id(&ctx->ui, text, 3u);
    (void)dgus_dgusii_text_set_mode_spacing(&ctx->ui, text, 0u, 1u);
    (void)dgus_dgusii_text_set_vertical_spacing(&ctx->ui, text, 2u);
    (void)dgus_dgusii_text_set_text_ascii(&ctx->ui, text, "DGUSII text");
    (void)dgus_dgusii_text_hide(&ctx->ui, text);
    (void)dgus_dgusii_text_show(&ctx->ui, text);

    return text;
}
