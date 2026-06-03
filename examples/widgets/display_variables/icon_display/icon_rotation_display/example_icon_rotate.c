#include "example_widgets.h"

#define EXAMPLE_ICON_ROTATE_ID 8u
#define EXAMPLE_ICON_ROTATE_VP 0x1140u
#define EXAMPLE_ICON_ROTATE_SP 0x80A0u

dgus_icon_rotate_t *dgus_example_icon_rotate_create(dgus_example_ctx_t *ctx)
{
    dgus_icon_rotate_t *rot;
    dgus_point_t icon_center;
    dgus_point_t screen_center;

    if (ctx == NULL) {
        return NULL;
    }

    rot = dgus_icon_rotate_create(&ctx->page,
                                  EXAMPLE_ICON_ROTATE_ID,
                                  EXAMPLE_ICON_ROTATE_VP,
                                  EXAMPLE_ICON_ROTATE_SP);
    if (rot == NULL) {
        return NULL;
    }

    icon_center.x = 24;
    icon_center.y = 24;
    screen_center.x = 160;
    screen_center.y = 120;

    (void)dgus_icon_rotate_set_icon(&ctx->ui, rot, 220u, 1u);
    (void)dgus_icon_rotate_set_centers(&ctx->ui,
                                       rot,
                                       icon_center,
                                       screen_center);
    (void)dgus_icon_rotate_set_value_range(&ctx->ui, rot, 0u, 1000u);
    (void)dgus_icon_rotate_set_angle_range(&ctx->ui, rot, 0u, 720u);
    (void)dgus_icon_rotate_set_value(&ctx->ui, rot, 250u);

    return rot;
}
