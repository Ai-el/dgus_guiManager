#include "example_widgets.h"

#define EXAMPLE_BUTTON_ID 1u
#define EXAMPLE_BUTTON_VP 0x1000u

static void example_button_clicked_cb(void *user_data)
{
    bool *clicked = (bool *)user_data;

    if (clicked != NULL) {
        *clicked = true;
    }

    /* Handle the button action here, or post a message to the application. */
}

static void example_button_trigger_cb(const dgus_ui_event_t *event,
                                      void *user_data)
{
    (void)user_data;

    if (event == NULL) {
        return;
    }

    /* This callback is routed from screen-originated VP uploads. Use
     * event->code to distinguish VALUE_CHANGED, CLICKED, or touch-state events.
     */
}

static void example_button_param_cb(const dgus_widget_param_event_t *event,
                                    void *user_data)
{
    (void)user_data;

    if (event == NULL) {
        return;
    }

    /* This callback runs after a local setter writes the button VP, for
     * example when auto-clear is enabled and the library writes 0 to the VP.
     */
}

dgus_button_t *dgus_example_button_create(dgus_example_ctx_t *ctx)
{
    dgus_button_t *button;
    static bool button_clicked;

    if (ctx == NULL) {
        return NULL;
    }

    button = dgus_button_create(&ctx->page,
                                EXAMPLE_BUTTON_ID,
                                EXAMPLE_BUTTON_VP);
    if (button == NULL) {
        return NULL;
    }

    dgus_button_set_key_value(button, 0x0001u);
    dgus_button_set_auto_clear(button, false);
    dgus_button_set_clicked_callback(button,
                                     example_button_clicked_cb,
                                     &button_clicked);
    dgus_button_set_trigger_callback(button, example_button_trigger_cb, NULL);
    dgus_button_set_param_changed_callback(button,
                                           example_button_param_cb,
                                           NULL);
    (void)dgus_obj_read(&ctx->ui, &button->obj);

    return button;
}
