#include "example_widgets.h"

#define EXAMPLE_ICON_ROLLER_ID 21u
#define EXAMPLE_ICON_ROLLER_VP 0x1260u
#define EXAMPLE_ICON_ROLLER_SP 0x8300u

dgus_icon_roller_t *dgus_example_icon_roller_create(dgus_example_ctx_t *ctx)
{
    dgus_icon_roller_t *roller;

    if (ctx == NULL) {
        return NULL;
    }

    roller = dgus_icon_roller_create(&ctx->page,
                                     EXAMPLE_ICON_ROLLER_ID,
                                     EXAMPLE_ICON_ROLLER_VP,
                                     EXAMPLE_ICON_ROLLER_SP);
    if (roller == NULL) {
        return NULL;
    }

    (void)dgus_icon_roller_set_adjust_data_mode(&ctx->ui, roller, 0u, 0u);
    (void)dgus_icon_roller_set_step_or_string_vp(&ctx->ui, roller, 0x1270u);
    (void)dgus_icon_roller_set_range(&ctx->ui, roller, 0, 9);
    (void)dgus_icon_roller_set_mode_speed(&ctx->ui, roller, 0u, 3u);
    (void)dgus_icon_roller_set_font_id(&ctx->ui, roller, 4u);
    (void)dgus_icon_roller_set_selected_pos(&ctx->ui, roller, 560, 120);
    (void)dgus_icon_roller_set_line_heights(&ctx->ui, roller, 24u, 24u, 24u, 24u);
    (void)dgus_icon_roller_set_brightness_height(&ctx->ui, roller, 0x60u, 28u);
    (void)dgus_icon_roller_set_value(&ctx->ui, roller, 3);
    (void)dgus_icon_roller_hide(&ctx->ui, roller);
    (void)dgus_icon_roller_show(&ctx->ui, roller);

    return roller;
}
