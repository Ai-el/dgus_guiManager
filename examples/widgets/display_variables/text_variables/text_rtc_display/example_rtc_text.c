#include "example_widgets.h"

#define EXAMPLE_RTC_TEXT_ID 15u
#define EXAMPLE_RTC_TEXT_SP 0x8240u

dgus_rtc_text_t *dgus_example_rtc_text_create(dgus_example_ctx_t *ctx)
{
    dgus_rtc_text_t *rtc;

    if (ctx == NULL) {
        return NULL;
    }

    rtc = dgus_rtc_text_create(&ctx->page,
                               EXAMPLE_RTC_TEXT_ID,
                               EXAMPLE_RTC_TEXT_SP);
    if (rtc == NULL) {
        return NULL;
    }

    (void)dgus_rtc_text_set_pos(&ctx->ui, rtc, 280, 40);
    (void)dgus_rtc_text_set_color(&ctx->ui, rtc, 0xFFFFu);
    (void)dgus_rtc_text_set_font(&ctx->ui, rtc, 0u, 16u);
    (void)dgus_rtc_text_set_format(&ctx->ui, rtc, "Y-M-D H:Q:S");

    return rtc;
}
