#include "example_widgets.h"

#define EXAMPLE_AREA_BRIGHTNESS_ID 28u
#define EXAMPLE_AREA_BRIGHTNESS_VP 0x1340u
#define EXAMPLE_AREA_BRIGHTNESS_SP 0x8480u

dgus_area_brightness_t *dgus_example_area_brightness_create(dgus_example_ctx_t *ctx)
{
    dgus_area_brightness_t *area_brightness;
    dgus_rect_t area;

    if (ctx == NULL) {
        return NULL;
    }

    area_brightness = dgus_area_brightness_create(&ctx->page,
                                                  EXAMPLE_AREA_BRIGHTNESS_ID,
                                                  EXAMPLE_AREA_BRIGHTNESS_VP,
                                                  EXAMPLE_AREA_BRIGHTNESS_SP);
    if (area_brightness == NULL) {
        return NULL;
    }

    area.x0 = 720;
    area.y0 = 400;
    area.x1 = 790;
    area.y1 = 470;

    (void)dgus_area_brightness_set_area(&ctx->ui, area_brightness, area);
    (void)dgus_area_brightness_set_value(&ctx->ui,
                                         area_brightness,
                                         0x0080u);

    return area_brightness;
}
