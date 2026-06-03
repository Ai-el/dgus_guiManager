#include "example_widgets.h"

#define EXAMPLE_RTC_ANALOG_ID 16u
#define EXAMPLE_RTC_ANALOG_SP 0x8260u

dgus_rtc_analog_t *dgus_example_rtc_analog_create(dgus_example_ctx_t *ctx)
{
    dgus_rtc_analog_t *rtc;
    dgus_point_t hand_center;

    if (ctx == NULL) {
        return NULL;
    }

    rtc = dgus_rtc_analog_create(&ctx->page,
                                 EXAMPLE_RTC_ANALOG_ID,
                                 EXAMPLE_RTC_ANALOG_SP);
    if (rtc == NULL) {
        return NULL;
    }

    hand_center.x = 8;
    hand_center.y = 36;

    (void)dgus_rtc_analog_set_center(&ctx->ui, rtc, 320, 120);
    (void)dgus_rtc_analog_set_hand(&ctx->ui,
                                   rtc,
                                   DGUS_RTC_ANALOG_SP_HOUR_ICON,
                                   DGUS_RTC_ANALOG_SP_HOUR_CENTER,
                                   500u,
                                   hand_center);
    (void)dgus_rtc_analog_set_hand(&ctx->ui,
                                   rtc,
                                   DGUS_RTC_ANALOG_SP_MINUTE_ICON,
                                   DGUS_RTC_ANALOG_SP_MINUTE_CENTER,
                                   501u,
                                   hand_center);
    (void)dgus_rtc_analog_set_hand(&ctx->ui,
                                   rtc,
                                   DGUS_RTC_ANALOG_SP_SECOND_ICON,
                                   DGUS_RTC_ANALOG_SP_SECOND_CENTER,
                                   502u,
                                   hand_center);
    (void)dgus_rtc_analog_set_icon_lib(&ctx->ui, rtc, 2u);

    return rtc;
}
