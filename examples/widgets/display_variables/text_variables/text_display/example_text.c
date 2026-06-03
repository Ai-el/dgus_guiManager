#include "example_widgets.h"

#define EXAMPLE_TEXT_ID 14u
#define EXAMPLE_TEXT_VP 0x1210u
#define EXAMPLE_TEXT_SP 0x8220u

dgus_text_t *dgus_example_text_create(dgus_example_ctx_t *ctx)
{
    dgus_text_t *text;
    dgus_rect_t box;

    if (ctx == NULL) {
        return NULL;
    }

    text = dgus_text_create(&ctx->page,
                            EXAMPLE_TEXT_ID,
                            EXAMPLE_TEXT_VP,
                            EXAMPLE_TEXT_SP,
                            16u);
    if (text == NULL) {
        return NULL;
    }

    box.x0 = 40;
    box.y0 = 210;
    box.x1 = 260;
    box.y1 = 242;

    dgus_text_set_terminate(text, true);
    (void)dgus_text_set_pos(&ctx->ui, text, 40, 210);
    (void)dgus_text_set_color(&ctx->ui, text, 0x07E0u);
    (void)dgus_text_set_box(&ctx->ui, text, box);
    (void)dgus_text_set_length(&ctx->ui, text, 32u);
    (void)dgus_text_set_font_ids(&ctx->ui, text, 0u, 1u);
    (void)dgus_text_set_font_dots(&ctx->ui, text, 16u, 16u);
    (void)dgus_text_set_text_ascii(&ctx->ui, text, "DGUS text");
    (void)dgus_text_hide(&ctx->ui, text);
    (void)dgus_text_show(&ctx->ui, text);

    return text;
}
