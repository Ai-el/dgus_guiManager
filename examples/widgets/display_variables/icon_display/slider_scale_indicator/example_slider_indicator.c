#include "example_widgets.h"

#define EXAMPLE_SLIDER_ID 5u
#define EXAMPLE_SLIDER_VP 0x1120u
#define EXAMPLE_SLIDER_SP 0x8040u

dgus_slider_indicator_t *dgus_example_slider_indicator_create(dgus_example_ctx_t *ctx)
{
    dgus_slider_indicator_t *slider;

    if (ctx == NULL) {
        return NULL;
    }

    slider = dgus_slider_indicator_create(&ctx->page,
                                          EXAMPLE_SLIDER_ID,
                                          EXAMPLE_SLIDER_VP,
                                          EXAMPLE_SLIDER_SP);
    if (slider == NULL) {
        return NULL;
    }

    (void)dgus_slider_indicator_set_value_range(&ctx->ui, slider, 0u, 100u);
    (void)dgus_slider_indicator_set_axis_range(&ctx->ui, slider, 40u, 240u);
    (void)dgus_slider_indicator_set_icon(&ctx->ui, slider, 140u, 100u);
    (void)dgus_slider_indicator_set_direction(&ctx->ui,
                                              slider,
                                              DGUS_SLIDER_HORIZONTAL);
    (void)dgus_slider_indicator_set_value(&ctx->ui, slider, 45u);

    return slider;
}
