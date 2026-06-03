#ifndef DGUS_BASIC_GRAPH_H
#define DGUS_BASIC_GRAPH_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DGUS_GRAPH_END 0xFF00u

typedef enum {
    DGUS_GRAPH_DOT = 0x0001u,
    DGUS_GRAPH_POLYLINE = 0x0002u,
    DGUS_GRAPH_RECT = 0x0003u,
    DGUS_GRAPH_RECT_FILL = 0x0004u,
    DGUS_GRAPH_CIRCLE = 0x0005u,
    DGUS_GRAPH_PIC_COPY = 0x0006u,
    DGUS_GRAPH_ICON = 0x0007u,
    DGUS_GRAPH_AREA_FILL = 0x0008u,
    DGUS_GRAPH_SPECTRUM = 0x0009u,
    DGUS_GRAPH_LINE = 0x000Au,
    DGUS_GRAPH_XOR_RECT = 0x000Du,
    DGUS_GRAPH_BITMAP_2C = 0x000Eu,
    DGUS_GRAPH_ELLIPSE_ARC = 0x0011u,
    DGUS_GRAPH_BITMAP_4C = 0x0012u
} dgus_graph_cmd_t;

/* Basic graph display (14.BIN type 0x21). Data is written to VP.
 *
 * Parameters:
 * - vp is the graph command buffer base.
 * - max_words is the command-buffer capacity reserved in DGUS VP memory.
 * - graph words are raw DGUS drawing commands terminated by DGUS_GRAPH_END.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t max_words;
} dgus_basic_graph_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_basic_graph_t, dgus_basic_graph);

/* Create a basic graph display object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS VP command buffer base.
 * max_words: maximum graph command words reserved at vp.
 */
dgus_basic_graph_t *dgus_basic_graph_create(dgus_page_t *page,
                                            uint16_t id,
                                            uint16_t vp,
                                            uint16_t max_words);

/* Write raw graph words to the graph VP.
 * words: DGUS graph command words.
 * word_count: number of words to send, including terminator if required.
 */
dgus_status_t dgus_basic_graph_write_words(dgus_ui_t *ui,
                                           const dgus_basic_graph_t *graph,
                                           const uint16_t *words,
                                           uint8_t word_count);

/* Draw one rectangle using command 0x0003 and the 0xFF00 terminator. */
dgus_status_t dgus_basic_graph_draw_rect(dgus_ui_t *ui,
                                         const dgus_basic_graph_t *graph,
                                         dgus_rect_t rect,
                                         uint16_t color);

/* Draw one filled rectangle using command 0x0004 and the 0xFF00 terminator. */
dgus_status_t dgus_basic_graph_fill_rect(dgus_ui_t *ui,
                                         const dgus_basic_graph_t *graph,
                                         dgus_rect_t rect,
                                         uint16_t color);

/* Draw one circle using command 0x0005 and the 0xFF00 terminator. */
dgus_status_t dgus_basic_graph_draw_circle(dgus_ui_t *ui,
                                           const dgus_basic_graph_t *graph,
                                           dgus_point_t center,
                                           uint16_t radius,
                                           uint16_t color);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_BASIC_GRAPH_H */
