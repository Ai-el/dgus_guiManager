#include "example_widgets.h"

#define EXAMPLE_DATA_WINDOW_ID 19u
#define EXAMPLE_DATA_WINDOW_VP 0x1240u
#define EXAMPLE_DATA_WINDOW_SP 0x82C0u

dgus_data_window_t *dgus_example_data_window_create(dgus_example_ctx_t *ctx)
{
    dgus_data_window_t *window;

    if (ctx == NULL) {
        return NULL;
    }

    window = dgus_data_window_create(&ctx->page,
                                     EXAMPLE_DATA_WINDOW_ID,
                                     EXAMPLE_DATA_WINDOW_VP,
                                     EXAMPLE_DATA_WINDOW_SP);
    if (window == NULL) {
        return NULL;
    }

    (void)dgus_data_window_set_range(&ctx->ui, window, -20, 80);
    (void)dgus_data_window_set_digits(&ctx->ui, window, 3u, 0u);
    (void)dgus_data_window_set_count_mode(&ctx->ui, window, 5u, 0u);
    (void)dgus_data_window_set_center(&ctx->ui, window, 360, 180);
    (void)dgus_data_window_set_step(&ctx->ui, window, 1u);
    (void)dgus_data_window_set_text_style(&ctx->ui,
                                          window,
                                          16u,
                                          16u,
                                          0x8410u,
                                          24u,
                                          24u,
                                          0xFFFFu);
    (void)dgus_data_window_set_value(&ctx->ui, window, 25);
    (void)dgus_data_window_hide(&ctx->ui, window);
    (void)dgus_data_window_show(&ctx->ui, window);

    return window;
}
