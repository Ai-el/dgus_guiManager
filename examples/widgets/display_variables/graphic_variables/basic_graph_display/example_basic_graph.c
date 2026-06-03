#include "example_widgets.h"

#define EXAMPLE_BASIC_GRAPH_ID 24u
#define EXAMPLE_BASIC_GRAPH_VP 0x1300u

dgus_basic_graph_t *dgus_example_basic_graph_create(dgus_example_ctx_t *ctx)
{
    dgus_basic_graph_t *graph;
    dgus_rect_t rect;
    dgus_point_t center;

    if (ctx == NULL) {
        return NULL;
    }

    graph = dgus_basic_graph_create(&ctx->page,
                                    EXAMPLE_BASIC_GRAPH_ID,
                                    EXAMPLE_BASIC_GRAPH_VP,
                                    32u);
    if (graph == NULL) {
        return NULL;
    }

    rect.x0 = 400;
    rect.y0 = 300;
    rect.x1 = 520;
    rect.y1 = 360;
    center.x = 590;
    center.y = 330;

    (void)dgus_basic_graph_draw_rect(&ctx->ui, graph, rect, 0xFFFFu);
    (void)dgus_basic_graph_fill_rect(&ctx->ui, graph, rect, 0x07E0u);
    (void)dgus_basic_graph_draw_circle(&ctx->ui, graph, center, 24u, 0xF800u);

    return graph;
}
