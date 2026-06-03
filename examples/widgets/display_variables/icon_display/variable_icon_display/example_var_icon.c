#include "example_widgets.h"

#define EXAMPLE_VAR_ICON_ID 3u
#define EXAMPLE_VAR_ICON_VP 0x1100u
#define EXAMPLE_VAR_ICON_SP 0x8000u

static void example_var_icon_param_cb(const dgus_widget_param_event_t *event,
                                      void *user_data)
{
    (void)user_data;

    if (event == NULL) {
        return;
    }

    /* event->space tells whether the setter changed VP data or SP descriptor
     * parameters. event->address is the absolute DGUS word address written.
     */
}

dgus_var_icon_t *dgus_example_var_icon_create(dgus_example_ctx_t *ctx)
{
    dgus_var_icon_t *icon;

    if (ctx == NULL) {
        return NULL;
    }

    icon = dgus_var_icon_create(&ctx->page,
                                EXAMPLE_VAR_ICON_ID,
                                EXAMPLE_VAR_ICON_VP,
                                EXAMPLE_VAR_ICON_SP);
    if (icon == NULL) {
        return NULL;
    }

    dgus_var_icon_set_param_changed_callback(icon,
                                             example_var_icon_param_cb,
                                             NULL);

    (void)dgus_var_icon_set_value(&ctx->ui, icon, 2u);
    (void)dgus_var_icon_set_pos(&ctx->ui, icon, 40, 40);
    (void)dgus_var_icon_set_value_range(&ctx->ui, icon, 0u, 5u);
    (void)dgus_var_icon_set_icon_range(&ctx->ui, icon, 100u, 105u);
    (void)dgus_var_icon_set_library_mode(&ctx->ui,
                                         icon,
                                         1u,
                                         DGUS_ICON_TRANSPARENT);
    (void)dgus_var_icon_set_layer(&ctx->ui,
                                  icon,
                                  DGUS_LAYER_OVERWRITE,
                                  0xFFu);
    (void)dgus_var_icon_hide(&ctx->ui, icon);
    (void)dgus_var_icon_show(&ctx->ui, icon);

    return icon;
}
