#include "example_widgets.h"

#define EXAMPLE_GTF_TEXT_ID 22u
#define EXAMPLE_GTF_TEXT_VP 0x1270u
#define EXAMPLE_GTF_TEXT_SP 0x8320u

dgus_gtf_text_t *dgus_example_gtf_text_create(dgus_example_ctx_t *ctx)
{
    dgus_gtf_text_t *text;
    dgus_rect_t box;

    if (ctx == NULL) {
        return NULL;
    }

    text = dgus_gtf_text_create(&ctx->page,
                                EXAMPLE_GTF_TEXT_ID,
                                EXAMPLE_GTF_TEXT_VP,
                                EXAMPLE_GTF_TEXT_SP,
                                16u);
    if (text == NULL) {
        return NULL;
    }

    box.x0 = 540;
    box.y0 = 210;
    box.x1 = 760;
    box.y1 = 250;

    dgus_gtf_text_set_terminate(text, true);
    (void)dgus_gtf_text_set_ids(&ctx->ui, text, 1u, 5u);
    (void)dgus_gtf_text_set_pos(&ctx->ui, text, 540, 210);
    (void)dgus_gtf_text_set_box(&ctx->ui, text, box);
    (void)dgus_gtf_text_set_spacing(&ctx->ui, text, 1u, 2u);
    (void)dgus_gtf_text_set_display_mode(&ctx->ui, text, 0u);
    (void)dgus_gtf_text_set_text_ascii(&ctx->ui, text, "GTF text");
    (void)dgus_gtf_text_hide(&ctx->ui, text);
    (void)dgus_gtf_text_show(&ctx->ui, text);

    return text;
}
