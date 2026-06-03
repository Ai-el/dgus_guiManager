#include "example_widgets.h"

#define EXAMPLE_AREA_SCROLL_ID 26u
#define EXAMPLE_AREA_SCROLL_VP 0x1320u
#define EXAMPLE_AREA_SCROLL_SP 0x8440u

dgus_area_scroll_t *dgus_example_area_scroll_create(dgus_example_ctx_t *ctx)
{
    dgus_area_scroll_t *scroll;
    dgus_rect_t area;

    if (ctx == NULL) {
        return NULL;
    }

    scroll = dgus_area_scroll_create(&ctx->page,
                                     EXAMPLE_AREA_SCROLL_ID,
                                     EXAMPLE_AREA_SCROLL_VP,
                                     EXAMPLE_AREA_SCROLL_SP);
    if (scroll == NULL) {
        return NULL;
    }

    area.x0 = 400;
    area.y0 = 420;
    area.x1 = 700;
    area.y1 = 460;

    (void)dgus_area_scroll_set_area(&ctx->ui, scroll, area);
    (void)dgus_area_scroll_set_speed(&ctx->ui, scroll, 4u);
    (void)dgus_area_scroll_set_direction(&ctx->ui, scroll, DGUS_SCROLL_LEFT);

    return scroll;
}
