#include "example_widgets.h"

#define EXAMPLE_BIT_ICON_ID 9u
#define EXAMPLE_BIT_ICON_VP 0x1150u
#define EXAMPLE_BIT_ICON_SP 0x80C0u

dgus_bit_icon_t *dgus_example_bit_icon_create(dgus_example_ctx_t *ctx)
{
    dgus_bit_icon_t *icon;

    if (ctx == NULL) {
        return NULL;
    }

    icon = dgus_bit_icon_create(&ctx->page,
                                EXAMPLE_BIT_ICON_ID,
                                EXAMPLE_BIT_ICON_VP,
                                EXAMPLE_BIT_ICON_SP);
    if (icon == NULL) {
        return NULL;
    }

    (void)dgus_bit_icon_set_active_bits(&ctx->ui, icon, 0x000Fu);
    (void)dgus_bit_icon_set_pos(&ctx->ui, icon, 40, 132);
    (void)dgus_bit_icon_set_step(&ctx->ui, icon, 24, 0);
    (void)dgus_bit_icon_set_icon_ids(&ctx->ui, icon, 300u, 303u, 310u, 313u);
    (void)dgus_bit_icon_set_bits(&ctx->ui, icon, 0x0005u);

    return icon;
}
