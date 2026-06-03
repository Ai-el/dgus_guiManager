#include "example_widgets.h"

#define EXAMPLE_PROGRESS_ID 25u
#define EXAMPLE_PROGRESS_VP 0x1310u
#define EXAMPLE_PROGRESS_SP 0x8420u

dgus_progress_t *dgus_example_progress_create(dgus_example_ctx_t *ctx)
{
    dgus_progress_t *bar;
    dgus_rect_t area;

    if (ctx == NULL) {
        return NULL;
    }

    bar = dgus_progress_create(&ctx->page,
                               EXAMPLE_PROGRESS_ID,
                               EXAMPLE_PROGRESS_VP,
                               EXAMPLE_PROGRESS_SP);
    if (bar == NULL) {
        return NULL;
    }

    area.x0 = 400;
    area.y0 = 380;
    area.x1 = 700;
    area.y1 = 410;

    dgus_progress_set_range(bar, 0, 100);
    (void)dgus_progress_set_area(&ctx->ui, bar, area);
    (void)dgus_progress_set_colors(&ctx->ui,
                                   bar,
                                   0xFFFFu,
                                   0x07E0u,
                                   0x0000u);
    (void)dgus_progress_set_value_limits(&ctx->ui, bar, 0, 100);
    (void)dgus_progress_set_mode(&ctx->ui,
                                 bar,
                                 DGUS_PROGRESS_FRAME_FILL_BG,
                                 DGUS_PROGRESS_RIGHT,
                                 true);
    (void)dgus_progress_set_return_vp(&ctx->ui, bar, 0x1311u);
    (void)dgus_progress_set_value(&ctx->ui, bar, 60u);

    return bar;
}
