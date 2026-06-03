#include "example_widgets.h"

#include <stdio.h>
#include <string.h>

static dgus_status_t example_uart_write(const uint8_t *data,
                                        size_t len,
                                        void *user_data)
{
    size_t i;

    (void)user_data;

    printf("TX %u:", (unsigned)len);
    for (i = 0u; i < len; i++) {
        printf(" %02X", data[i]);
    }
    printf("\n");

    return DGUS_OK;
}

static const char *example_event_name(dgus_ui_event_code_t code)
{
    switch (code) {
    case DGUS_UI_EVENT_PAGE_LOAD:
        return "PAGE_LOAD";
    case DGUS_UI_EVENT_PAGE_UNLOAD:
        return "PAGE_UNLOAD";
    case DGUS_UI_EVENT_VALUE_CHANGED:
        return "VALUE_CHANGED";
    case DGUS_UI_EVENT_CLICKED:
        return "CLICKED";
    case DGUS_UI_EVENT_TOUCH_PRESSED:
        return "TOUCH_PRESSED";
    case DGUS_UI_EVENT_TOUCH_PRESSING:
        return "TOUCH_PRESSING";
    case DGUS_UI_EVENT_TOUCH_RELEASED:
        return "TOUCH_RELEASED";
    case DGUS_UI_EVENT_WRITE_OK:
        return "WRITE_OK";
    case DGUS_UI_EVENT_FRAME:
        return "FRAME";
    default:
        return "UNKNOWN";
    }
}

static void example_ui_event_cb(const dgus_ui_event_t *event, void *user_data)
{
    (void)user_data;

    if (event == NULL) {
        return;
    }

    printf("EVENT %-15s vp=0x%04X value=0x%04X",
           example_event_name(event->code),
           (unsigned)event->vp,
           (unsigned)event->value);

    if (event->target != NULL) {
        printf(" target_id=%u", (unsigned)event->target->id);
    }
    if ((event->code == DGUS_UI_EVENT_TOUCH_PRESSED) ||
        (event->code == DGUS_UI_EVENT_TOUCH_PRESSING) ||
        (event->code == DGUS_UI_EVENT_TOUCH_RELEASED)) {
        printf(" touch=(%u,%u)", (unsigned)event->x, (unsigned)event->y);
    }

    printf("\n");
}

void dgus_example_context_init(dgus_example_ctx_t *ctx,
                               const dgus_port_t *port)
{
    if (ctx == NULL) {
        return;
    }

    memset(ctx, 0, sizeof(*ctx));
    dgus_init(&ctx->dev, port);
    dgus_page_init(&ctx->page,
                   1u,
                   "main",
                   ctx->objects,
                   DGUS_EXAMPLE_OBJECT_CAPACITY,
                   ctx->page_pool,
                   sizeof(ctx->page_pool),
                   NULL,
                   NULL);
    dgus_ui_init(&ctx->ui, &ctx->dev, &ctx->page, 1u);
    dgus_ui_set_event_callback(&ctx->ui, example_ui_event_cb, NULL);

    dgus_anim_manager_init(&ctx->anim_manager,
                           ctx->anim_items,
                           DGUS_EXAMPLE_ANIM_CAPACITY);
    dgus_runtime_init(&ctx->runtime,
                      &ctx->dev,
                      &ctx->ui,
                      &ctx->anim_manager);
    dgus_runtime_set_watch_table(&ctx->runtime,
                                 ctx->watches,
                                 DGUS_EXAMPLE_WATCH_CAPACITY);
    dgus_runtime_set_rx_buffer(&ctx->runtime,
                               ctx->rx_buffer,
                               sizeof(ctx->rx_buffer));
    dgus_runtime_set_page_poll(&ctx->runtime, 1000u);
    dgus_runtime_set_touch_poll(&ctx->runtime, 250u);
    dgus_runtime_set_poll_tx_budget(&ctx->runtime, 2u);

    (void)dgus_runtime_watch_add(&ctx->runtime, 0x1200u, 1u, 500u);
}

void dgus_example_run_all_widget_demos(dgus_example_ctx_t *ctx)
{
    if (ctx == NULL) {
        return;
    }

    (void)dgus_example_button_create(ctx);
    (void)dgus_example_switch_create(ctx);

    ctx->motion_icon = dgus_example_var_icon_create(ctx);
    (void)dgus_example_anim_icon_create(ctx);
    (void)dgus_example_slider_indicator_create(ctx);
    (void)dgus_example_art_text_create(ctx);
    (void)dgus_example_pic_anim_create(ctx);
    (void)dgus_example_icon_rotate_create(ctx);
    (void)dgus_example_bit_icon_create(ctx);
    (void)dgus_example_jpeg_pan_create(ctx);
    (void)dgus_example_jpeg_create(ctx);
    (void)dgus_example_batch_icon_create(ctx);

    (void)dgus_example_data_var_create(ctx);
    (void)dgus_example_text_create(ctx);
    (void)dgus_example_rtc_text_create(ctx);
    (void)dgus_example_rtc_analog_create(ctx);
    (void)dgus_example_hex_create(ctx);
    (void)dgus_example_text_scroll_create(ctx);
    (void)dgus_example_data_window_create(ctx);
    (void)dgus_example_dgusii_text_create(ctx);
    (void)dgus_example_icon_roller_create(ctx);
    (void)dgus_example_gtf_text_create(ctx);

    (void)dgus_example_trend_curve_create(ctx);
    (void)dgus_example_basic_graph_create(ctx);
    (void)dgus_example_progress_create(ctx);
    (void)dgus_example_area_scroll_create(ctx);
    (void)dgus_example_qr_create(ctx);
    (void)dgus_example_area_brightness_create(ctx);

    (void)dgus_example_data_transfer_create(ctx);
    (void)dgus_example_video_create(ctx);
}

static int32_t example_smoothstep_path(const dgus_anim_t *anim)
{
    int32_t p = dgus_anim_get_progress(anim);

    /* Smoothstep easing with the same callback shape as LVGL animation paths. */
    return (p * p * (3 * DGUS_ANIM_PROGRESS_MAX - (2 * p))) /
           (DGUS_ANIM_PROGRESS_MAX * DGUS_ANIM_PROGRESS_MAX);
}

void dgus_example_start_motion(dgus_example_ctx_t *ctx)
{
    if ((ctx == NULL) || (ctx->motion_icon == NULL)) {
        return;
    }

    dgus_anim_var_icon_move_init(&ctx->motion_move,
                                 &ctx->motion_anim,
                                 &ctx->ui,
                                 ctx->motion_icon,
                                 40,
                                 40,
                                 240,
                                 120,
                                 1000u,
                                 example_smoothstep_path);
    dgus_anim_set_playback(&ctx->motion_anim, true);
    dgus_anim_set_repeat_count(&ctx->motion_anim, 2u);
    (void)dgus_anim_start(&ctx->anim_manager, &ctx->motion_anim);
}

static void example_feed_one_word(dgus_example_ctx_t *ctx,
                                  uint16_t vp,
                                  uint16_t value)
{
    uint8_t frame[9];

    frame[0] = DGUS_FRAME_HEAD_H;
    frame[1] = DGUS_FRAME_HEAD_L;
    frame[2] = 0x06u;
    frame[3] = DGUS_CMD_READ_VAR;
    dgus_put_u16_be(&frame[4], vp);
    frame[6] = 0x01u;
    dgus_put_u16_be(&frame[7], value);

    (void)dgus_runtime_rx_write(&ctx->runtime, frame, sizeof(frame));
}

static void example_feed_touch_state(dgus_example_ctx_t *ctx,
                                     uint8_t state,
                                     uint16_t x,
                                     uint16_t y)
{
    uint8_t frame[13];

    frame[0] = DGUS_FRAME_HEAD_H;
    frame[1] = DGUS_FRAME_HEAD_L;
    frame[2] = 0x0Au;
    frame[3] = DGUS_CMD_READ_VAR;
    dgus_put_u16_be(&frame[4], DGUS_SYS_ADDR_TOUCH_STATE);
    frame[6] = 0x03u;
    frame[7] = 0x5Au;
    frame[8] = state;
    dgus_put_u16_be(&frame[9], x);
    dgus_put_u16_be(&frame[11], y);

    (void)dgus_runtime_rx_write(&ctx->runtime, frame, sizeof(frame));
}

int main(void)
{
    dgus_example_ctx_t ctx;
    dgus_port_t port;
    unsigned i;

    port.write = example_uart_write;
    port.user_data = NULL;

    dgus_example_context_init(&ctx, &port);

    (void)dgus_ui_set_page(&ctx.ui, 1u);
    dgus_example_run_all_widget_demos(&ctx);
    dgus_example_start_motion(&ctx);

    /* These bytes are the same frames a board could push from a UART idle ISR.
     * dgus_timer_handler() later parses them and dispatches callbacks.
     */
    example_feed_one_word(&ctx, 0x1000u, 0x0001u);
    example_feed_one_word(&ctx, 0x1001u, 0x0001u);
    example_feed_touch_state(&ctx, 0x01u, 120u, 80u);

    for (i = 0u; i < 12u; i++) {
        dgus_tick_inc(100u);
        (void)dgus_timer_handler(&ctx.runtime);
    }

    return 0;
}
