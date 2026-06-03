#include "example_widgets.h"

#define EXAMPLE_TEXT_SCROLL_ID 18u
#define EXAMPLE_TEXT_SCROLL_VP 0x1230u
#define EXAMPLE_TEXT_SCROLL_SP 0x82A0u

dgus_text_scroll_t *dgus_example_text_scroll_create(dgus_example_ctx_t *ctx)
{
    dgus_text_scroll_t *scroll;
    dgus_rect_t box;

    if (ctx == NULL) {
        return NULL;
    }

    scroll = dgus_text_scroll_create(&ctx->page,
                                     EXAMPLE_TEXT_SCROLL_ID,
                                     EXAMPLE_TEXT_SCROLL_VP,
                                     EXAMPLE_TEXT_SCROLL_SP,
                                     24u);
    if (scroll == NULL) {
        return NULL;
    }

    box.x0 = 40;
    box.y0 = 252;
    box.x1 = 300;
    box.y1 = 284;

    dgus_text_scroll_set_terminate(scroll, true);
    (void)dgus_text_scroll_set_rolling(&ctx->ui,
                                       scroll,
                                       DGUS_TEXT_SCROLL_MODE_LEFT,
                                       2u);
    (void)dgus_text_scroll_set_align(&ctx->ui,
                                     scroll,
                                     DGUS_TEXT_SCROLL_ALIGN_CENTER);
    (void)dgus_text_scroll_set_color(&ctx->ui, scroll, 0x001Fu);
    (void)dgus_text_scroll_set_box(&ctx->ui, scroll, box);
    (void)dgus_text_scroll_set_font_ids(&ctx->ui, scroll, 0u, 1u);
    (void)dgus_text_scroll_set_font_dots(&ctx->ui, scroll, 16u, 16u);
    (void)dgus_text_scroll_set_encode_spacing(&ctx->ui, scroll, 0u, 1u);
    (void)dgus_text_scroll_set_text_ascii(&ctx->ui,
                                          scroll,
                                          "scrolling text example");
    (void)dgus_text_scroll_hide(&ctx->ui, scroll);
    (void)dgus_text_scroll_show(&ctx->ui, scroll);

    return scroll;
}
