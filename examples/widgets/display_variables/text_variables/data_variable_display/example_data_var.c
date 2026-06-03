#include "example_widgets.h"

#define EXAMPLE_DATA_VAR_ID 13u
#define EXAMPLE_DATA_VAR_VP 0x1200u
#define EXAMPLE_DATA_VAR_SP 0x8200u

dgus_data_var_t *dgus_example_data_var_create(dgus_example_ctx_t *ctx)
{
    dgus_data_var_t *data;

    if (ctx == NULL) {
        return NULL;
    }

    data = dgus_data_var_create(&ctx->page,
                                EXAMPLE_DATA_VAR_ID,
                                EXAMPLE_DATA_VAR_VP,
                                EXAMPLE_DATA_VAR_SP);
    if (data == NULL) {
        return NULL;
    }

    dgus_data_var_set_range(data, -400, 1250);
    (void)dgus_data_var_set_pos(&ctx->ui, data, 40, 176);
    (void)dgus_data_var_set_color(&ctx->ui, data, 0xF800u);
    (void)dgus_data_var_set_font(&ctx->ui, data, 0u, 24u);
    (void)dgus_data_var_set_integer_digits(&ctx->ui,
                                           data,
                                           DGUS_ALIGN_RIGHT,
                                           4u);
    (void)dgus_data_var_set_decimal_mode(&ctx->ui,
                                         data,
                                         1u,
                                         DGUS_DATA_WORD);
    (void)dgus_data_var_set_u16(&ctx->ui, data, 253u);
    (void)dgus_data_var_hide(&ctx->ui, data);
    (void)dgus_data_var_show(&ctx->ui, data);

    return data;
}
