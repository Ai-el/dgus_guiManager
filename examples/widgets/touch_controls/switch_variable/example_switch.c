#include "example_widgets.h"

#define EXAMPLE_SWITCH_ID 2u
#define EXAMPLE_SWITCH_VP 0x1001u

dgus_switch_t *dgus_example_switch_create(dgus_example_ctx_t *ctx)
{
    dgus_switch_t *sw;

    if (ctx == NULL) {
        return NULL;
    }

    sw = dgus_switch_create(&ctx->page,
                            EXAMPLE_SWITCH_ID,
                            EXAMPLE_SWITCH_VP);
    if (sw == NULL) {
        return NULL;
    }

    dgus_switch_set_values(sw, 0x0000u, 0x0001u);
    (void)dgus_switch_set_checked(&ctx->ui, sw, true);

    return sw;
}
