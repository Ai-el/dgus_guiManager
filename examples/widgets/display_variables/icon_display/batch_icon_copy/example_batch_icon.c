#include "example_widgets.h"

#define EXAMPLE_BATCH_ICON_ID 12u
#define EXAMPLE_BATCH_ICON_VP 0x1180u
#define EXAMPLE_BATCH_ICON_SP 0x8120u

dgus_batch_icon_t *dgus_example_batch_icon_create(dgus_example_ctx_t *ctx)
{
    dgus_batch_icon_t *icon;
    dgus_batch_icon_item_t item;

    if (ctx == NULL) {
        return NULL;
    }

    icon = dgus_batch_icon_create(&ctx->page,
                                  EXAMPLE_BATCH_ICON_ID,
                                  EXAMPLE_BATCH_ICON_VP,
                                  EXAMPLE_BATCH_ICON_SP,
                                  2u);
    if (icon == NULL) {
        return NULL;
    }

    item.valid_marker = 0x5Au;
    item.icon_id = 1u;
    item.mode_filter = 0u;
    item.brightness = 0xFFu;

    (void)dgus_batch_icon_set_sources(&ctx->ui,
                                      icon,
                                      EXAMPLE_BATCH_ICON_VP,
                                      0x1190u,
                                      0x11A0u);
    (void)dgus_batch_icon_set_count(&ctx->ui, icon, 2u);
    (void)dgus_batch_icon_set_background(&ctx->ui, icon, 0u, 1u, 400u);
    (void)dgus_batch_icon_set_item(&ctx->ui, icon, 0u, item);

    return icon;
}
