#include "example_widgets.h"

#define EXAMPLE_HEX_ID 17u
#define EXAMPLE_HEX_VP 0x1220u
#define EXAMPLE_HEX_SP 0x8280u

dgus_hex_t *dgus_example_hex_create(dgus_example_ctx_t *ctx)
{
    dgus_hex_t *hex;
    const uint8_t bytes[] = { 0x12u, 0x34u, 0xABu, 0xCDu };
    const uint16_t words[] = { 0x8280u, 0x0004u };

    if (ctx == NULL) {
        return NULL;
    }

    hex = dgus_hex_create(&ctx->page,
                          EXAMPLE_HEX_ID,
                          EXAMPLE_HEX_VP,
                          EXAMPLE_HEX_SP,
                          sizeof(bytes));
    if (hex == NULL) {
        return NULL;
    }

    (void)dgus_hex_desc_write_words(&ctx->ui, hex, 0u, words, 2u);
    (void)dgus_hex_write_bytes(&ctx->ui, hex, bytes, sizeof(bytes));

    return hex;
}
