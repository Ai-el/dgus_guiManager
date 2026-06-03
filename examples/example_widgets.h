#ifndef DGUS_EXAMPLE_WIDGETS_H
#define DGUS_EXAMPLE_WIDGETS_H

#include "dgus_runtime.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DGUS_EXAMPLE_OBJECT_CAPACITY 64u
#define DGUS_EXAMPLE_PAGE_POOL_SIZE  8192u
#define DGUS_EXAMPLE_ANIM_CAPACITY   4u
#define DGUS_EXAMPLE_WATCH_CAPACITY  8u
#define DGUS_EXAMPLE_RX_BUFFER_SIZE  256u

typedef struct {
    dgus_t dev;
    dgus_ui_t ui;
    dgus_runtime_t runtime;
    dgus_page_t page;
    dgus_obj_t *objects[DGUS_EXAMPLE_OBJECT_CAPACITY];
    uint8_t page_pool[DGUS_EXAMPLE_PAGE_POOL_SIZE];
    dgus_anim_manager_t anim_manager;
    dgus_anim_t anim_items[DGUS_EXAMPLE_ANIM_CAPACITY];
    dgus_runtime_watch_t watches[DGUS_EXAMPLE_WATCH_CAPACITY];
    uint8_t rx_buffer[DGUS_EXAMPLE_RX_BUFFER_SIZE];
    dgus_anim_move_t motion_move;
    dgus_anim_t motion_anim;
    dgus_var_icon_t *motion_icon;
} dgus_example_ctx_t;

void dgus_example_context_init(dgus_example_ctx_t *ctx,
                               const dgus_port_t *port);
void dgus_example_run_all_widget_demos(dgus_example_ctx_t *ctx);
void dgus_example_start_motion(dgus_example_ctx_t *ctx);

dgus_button_t *dgus_example_button_create(dgus_example_ctx_t *ctx);
dgus_switch_t *dgus_example_switch_create(dgus_example_ctx_t *ctx);

dgus_var_icon_t *dgus_example_var_icon_create(dgus_example_ctx_t *ctx);
dgus_anim_icon_t *dgus_example_anim_icon_create(dgus_example_ctx_t *ctx);
dgus_slider_indicator_t *dgus_example_slider_indicator_create(dgus_example_ctx_t *ctx);
dgus_art_text_t *dgus_example_art_text_create(dgus_example_ctx_t *ctx);
dgus_pic_anim_t *dgus_example_pic_anim_create(dgus_example_ctx_t *ctx);
dgus_icon_rotate_t *dgus_example_icon_rotate_create(dgus_example_ctx_t *ctx);
dgus_bit_icon_t *dgus_example_bit_icon_create(dgus_example_ctx_t *ctx);
dgus_jpeg_pan_t *dgus_example_jpeg_pan_create(dgus_example_ctx_t *ctx);
dgus_jpeg_t *dgus_example_jpeg_create(dgus_example_ctx_t *ctx);
dgus_batch_icon_t *dgus_example_batch_icon_create(dgus_example_ctx_t *ctx);

dgus_data_var_t *dgus_example_data_var_create(dgus_example_ctx_t *ctx);
dgus_text_t *dgus_example_text_create(dgus_example_ctx_t *ctx);
dgus_rtc_text_t *dgus_example_rtc_text_create(dgus_example_ctx_t *ctx);
dgus_rtc_analog_t *dgus_example_rtc_analog_create(dgus_example_ctx_t *ctx);
dgus_hex_t *dgus_example_hex_create(dgus_example_ctx_t *ctx);
dgus_text_scroll_t *dgus_example_text_scroll_create(dgus_example_ctx_t *ctx);
dgus_data_window_t *dgus_example_data_window_create(dgus_example_ctx_t *ctx);
dgus_dgusii_text_t *dgus_example_dgusii_text_create(dgus_example_ctx_t *ctx);
dgus_icon_roller_t *dgus_example_icon_roller_create(dgus_example_ctx_t *ctx);
dgus_gtf_text_t *dgus_example_gtf_text_create(dgus_example_ctx_t *ctx);

dgus_trend_curve_t *dgus_example_trend_curve_create(dgus_example_ctx_t *ctx);
dgus_basic_graph_t *dgus_example_basic_graph_create(dgus_example_ctx_t *ctx);
dgus_progress_t *dgus_example_progress_create(dgus_example_ctx_t *ctx);
dgus_area_scroll_t *dgus_example_area_scroll_create(dgus_example_ctx_t *ctx);
dgus_qr_t *dgus_example_qr_create(dgus_example_ctx_t *ctx);
dgus_area_brightness_t *dgus_example_area_brightness_create(dgus_example_ctx_t *ctx);

dgus_data_transfer_t *dgus_example_data_transfer_create(dgus_example_ctx_t *ctx);
dgus_video_t *dgus_example_video_create(dgus_example_ctx_t *ctx);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_EXAMPLE_WIDGETS_H */
