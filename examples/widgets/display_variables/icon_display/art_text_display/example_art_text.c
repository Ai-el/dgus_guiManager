#include "example_widgets.h"

#define EXAMPLE_ART_TEXT_ID 6u
#define EXAMPLE_ART_TEXT_VP 0x1130u
#define EXAMPLE_ART_TEXT_SP 0x8060u

dgus_art_text_t *dgus_example_art_text_create(dgus_example_ctx_t *ctx)
{
    dgus_art_text_t *art;

    if (ctx == NULL) {
        return NULL;
    }

    art = dgus_art_text_create(&ctx->page,
                               EXAMPLE_ART_TEXT_ID,
                               EXAMPLE_ART_TEXT_VP,
                               EXAMPLE_ART_TEXT_SP,
                               1u);
    if (art == NULL) {
        return NULL;
    }

    (void)dgus_art_text_set_pos(&ctx->ui, art, 40, 86);
    (void)dgus_art_text_set_icon0(&ctx->ui, art, 200u);
    (void)dgus_art_text_set_digits(&ctx->ui, art, 3u, 1u);
    (void)dgus_art_text_set_u16(&ctx->ui, art, 256u);

    return art;
}
