#include "dgus_rtc_text.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_rtc_text_t, dgus_rtc_text)
#include <string.h>

dgus_rtc_text_t *dgus_rtc_text_create(dgus_page_t *page,
                                      uint16_t id,
                                      uint16_t sp)
{
    dgus_rtc_text_t *rtc;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    rtc = (dgus_rtc_text_t *)dgus_page_alloc(page, sizeof(*rtc));
    if (rtc == NULL) {
        return NULL;
    }
    dgus_widget_init_obj(&rtc->obj, id, DGUS_OBJ_RTC_TEXT, DGUS_VP_NONE, 0u);
    rtc->sp = sp;
    if (dgus_page_add_obj(page, &rtc->obj) != DGUS_OK) {
        return NULL;
    }

    return rtc;
}

bool dgus_rtc_text_has_sp(const dgus_rtc_text_t *rtc)
{
    return (rtc != NULL) && dgus_widget_has_sp(rtc->sp);
}

dgus_status_t dgus_rtc_text_desc_write_u16(dgus_ui_t *ui,
                                           const dgus_rtc_text_t *rtc,
                                           uint16_t offset,
                                           uint16_t value)
{
    if (!dgus_rtc_text_has_sp(rtc)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &rtc->obj, rtc->sp, offset, value);
}

dgus_status_t dgus_rtc_text_set_pos(dgus_ui_t *ui,
                                    const dgus_rtc_text_t *rtc,
                                    int16_t x,
                                    int16_t y)
{
    dgus_point_t point;

    if (!dgus_rtc_text_has_sp(rtc)) {
        return DGUS_ERR_ARG;
    }

    point.x = x;
    point.y = y;
    return dgus_obj_desc_write_point_at(ui, &rtc->obj, rtc->sp, DGUS_RTC_TEXT_SP_POS, point);
}

dgus_status_t dgus_rtc_text_set_color(dgus_ui_t *ui,
                                      const dgus_rtc_text_t *rtc,
                                      uint16_t rgb565)
{
    return dgus_rtc_text_desc_write_u16(ui, rtc, DGUS_RTC_TEXT_SP_COLOR, rgb565);
}

dgus_status_t dgus_rtc_text_set_font(dgus_ui_t *ui,
                                     const dgus_rtc_text_t *rtc,
                                     uint8_t font_lib,
                                     uint8_t x_dots)
{
    return dgus_rtc_text_desc_write_u16(ui,
                                        rtc,
                                        DGUS_RTC_TEXT_SP_FONT,
                                        dgus_pack_u8(font_lib, x_dots));
}

dgus_status_t dgus_rtc_text_set_format(dgus_ui_t *ui,
                                       const dgus_rtc_text_t *rtc,
                                       const char *format)
{
    uint8_t bytes[DGUS_RTC_TEXT_FORMAT_MAX];
    size_t len;

    if (!dgus_rtc_text_has_sp(rtc) || (format == NULL)) {
        return DGUS_ERR_ARG;
    }

    memset(bytes, 0, sizeof(bytes));
    len = strlen(format);
    if (len >= sizeof(bytes)) {
        len = sizeof(bytes) - 1u;
    }
    memcpy(bytes, format, len);

    return dgus_obj_desc_write_bytes_at(ui, &rtc->obj, rtc->sp,
                                 DGUS_RTC_TEXT_SP_FORMAT,
                                 bytes,
                                 sizeof(bytes));
}
