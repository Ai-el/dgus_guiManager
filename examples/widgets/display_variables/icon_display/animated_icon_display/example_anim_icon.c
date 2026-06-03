#include "example_widgets.h"

#define EXAMPLE_ANIM_ICON_ID 4u
#define EXAMPLE_ANIM_ICON_VP 0x1110u
#define EXAMPLE_ANIM_ICON_SP 0x8020u

dgus_anim_icon_t *dgus_example_anim_icon_create(dgus_example_ctx_t *ctx)
{
    dgus_anim_icon_t *icon;

    if (ctx == NULL) {
        return NULL;
    }

    icon = dgus_anim_icon_create(&ctx->page,
                                 EXAMPLE_ANIM_ICON_ID,
                                 EXAMPLE_ANIM_ICON_VP,
                                 EXAMPLE_ANIM_ICON_SP);
    if (icon == NULL) {
        return NULL;
    }

    (void)dgus_anim_icon_set_values(&ctx->ui, icon, 0u, 1u);
    (void)dgus_anim_icon_set_icon_range(&ctx->ui, icon, 120u, 121u, 128u);
    (void)dgus_anim_icon_set_timing(&ctx->ui,
                                    icon,
                                    4u,
                                    DGUS_ANIM_ICON_LOOP);
    (void)dgus_anim_icon_set_pos(&ctx->ui, icon, 88, 40);
    (void)dgus_anim_icon_set_value(&ctx->ui, icon, 1u);

    return icon;
}
