#include "example_widgets.h"

#define EXAMPLE_TREND_CURVE_ID 23u
#define EXAMPLE_TREND_CURVE_SP 0x8400u

dgus_trend_curve_t *dgus_example_trend_curve_create(dgus_example_ctx_t *ctx)
{
    dgus_trend_curve_t *curve;
    dgus_rect_t area;
    const uint16_t samples[] = { 100u, 120u, 160u, 140u, 180u, 220u };

    if (ctx == NULL) {
        return NULL;
    }

    curve = dgus_trend_curve_create(&ctx->page,
                                    EXAMPLE_TREND_CURVE_ID,
                                    EXAMPLE_TREND_CURVE_SP,
                                    0u);
    if (curve == NULL) {
        return NULL;
    }

    area.x0 = 40;
    area.y0 = 300;
    area.x1 = 360;
    area.y1 = 440;

    (void)dgus_trend_curve_set_direction(&ctx->ui,
                                         curve,
                                         DGUS_TREND_LATEST_RIGHT);
    (void)dgus_trend_curve_set_area(&ctx->ui, curve, area);
    (void)dgus_trend_curve_set_y_axis(&ctx->ui, curve, 370u, 160u);
    (void)dgus_trend_curve_set_color(&ctx->ui, curve, 0x07E0u);
    (void)dgus_trend_curve_set_y_scale(&ctx->ui, curve, 256u);
    (void)dgus_trend_curve_set_channel_spacing(&ctx->ui, curve, 0u, 1u);
    (void)dgus_trend_curve_set_pixel_scale(&ctx->ui, curve, 0u);
    (void)dgus_trend_curve_write_samples(&ctx->ui,
                                         curve,
                                         samples,
                                         (uint8_t)(sizeof(samples) / sizeof(samples[0])));

    return curve;
}
