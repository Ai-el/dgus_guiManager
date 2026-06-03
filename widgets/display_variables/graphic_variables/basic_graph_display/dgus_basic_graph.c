#include "dgus_basic_graph.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_basic_graph_t, dgus_basic_graph)
dgus_basic_graph_t *dgus_basic_graph_create(dgus_page_t *page,
                                            uint16_t id,
                                            uint16_t vp,
                                            uint16_t max_words)
{
    dgus_basic_graph_t *graph;
    uint8_t words;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    graph = (dgus_basic_graph_t *)dgus_page_alloc(page, sizeof(*graph));
    if (graph == NULL) {
        return NULL;
    }

    words = (max_words > 255u) ? 255u : (uint8_t)max_words;
    dgus_widget_init_obj(&graph->obj, id, DGUS_OBJ_BASIC_GRAPH, vp, words);
    graph->max_words = max_words;
    if (dgus_page_add_obj(page, &graph->obj) != DGUS_OK) {
        return NULL;
    }

    return graph;
}

dgus_status_t dgus_basic_graph_write_words(dgus_ui_t *ui,
                                           const dgus_basic_graph_t *graph,
                                           const uint16_t *words,
                                           uint8_t word_count)
{
    if ((graph == NULL) || (words == NULL) || (word_count == 0u)) {
        return DGUS_ERR_ARG;
    }
    if ((graph->max_words != 0u) && (word_count > graph->max_words)) {
        return DGUS_ERR_OVERFLOW;
    }

    return dgus_obj_write_words(ui, &graph->obj, words, word_count);
}

dgus_status_t dgus_basic_graph_draw_rect(dgus_ui_t *ui,
                                         const dgus_basic_graph_t *graph,
                                         dgus_rect_t rect,
                                         uint16_t color)
{
    uint16_t words[8];

    words[0] = DGUS_GRAPH_RECT;
    words[1] = 1u;
    words[2] = (uint16_t)rect.x0;
    words[3] = (uint16_t)rect.y0;
    words[4] = (uint16_t)rect.x1;
    words[5] = (uint16_t)rect.y1;
    words[6] = color;
    words[7] = DGUS_GRAPH_END;

    return dgus_basic_graph_write_words(ui, graph, words, 8u);
}

dgus_status_t dgus_basic_graph_fill_rect(dgus_ui_t *ui,
                                         const dgus_basic_graph_t *graph,
                                         dgus_rect_t rect,
                                         uint16_t color)
{
    uint16_t words[8];

    words[0] = DGUS_GRAPH_RECT_FILL;
    words[1] = 1u;
    words[2] = (uint16_t)rect.x0;
    words[3] = (uint16_t)rect.y0;
    words[4] = (uint16_t)rect.x1;
    words[5] = (uint16_t)rect.y1;
    words[6] = color;
    words[7] = DGUS_GRAPH_END;

    return dgus_basic_graph_write_words(ui, graph, words, 8u);
}

dgus_status_t dgus_basic_graph_draw_circle(dgus_ui_t *ui,
                                           const dgus_basic_graph_t *graph,
                                           dgus_point_t center,
                                           uint16_t radius,
                                           uint16_t color)
{
    uint16_t words[7];

    words[0] = DGUS_GRAPH_CIRCLE;
    words[1] = 1u;
    words[2] = (uint16_t)center.x;
    words[3] = (uint16_t)center.y;
    words[4] = radius;
    words[5] = color;
    words[6] = DGUS_GRAPH_END;

    return dgus_basic_graph_write_words(ui, graph, words, 7u);
}
