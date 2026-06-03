#include "dgus_rtc_analog.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_rtc_analog_t, dgus_rtc_analog)
dgus_rtc_analog_t *dgus_rtc_analog_create(dgus_page_t *page,
                                          uint16_t id,
                                          uint16_t sp)
{
    dgus_rtc_analog_t *rtc;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    rtc = (dgus_rtc_analog_t *)dgus_page_alloc(page, sizeof(*rtc));
    if (rtc == NULL) {
        return NULL;
    }
    dgus_widget_init_obj(&rtc->obj, id, DGUS_OBJ_RTC_ANALOG, DGUS_VP_NONE, 0u);
    rtc->sp = sp;
    if (dgus_page_add_obj(page, &rtc->obj) != DGUS_OK) {
        return NULL;
    }

    return rtc;
}

bool dgus_rtc_analog_has_sp(const dgus_rtc_analog_t *rtc)
{
    return (rtc != NULL) && dgus_widget_has_sp(rtc->sp);
}

dgus_status_t dgus_rtc_analog_desc_write_u16(dgus_ui_t *ui,
                                             const dgus_rtc_analog_t *rtc,
                                             uint16_t offset,
                                             uint16_t value)
{
    if (!dgus_rtc_analog_has_sp(rtc)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &rtc->obj, rtc->sp, offset, value);
}

dgus_status_t dgus_rtc_analog_set_center(dgus_ui_t *ui,
                                         const dgus_rtc_analog_t *rtc,
                                         int16_t x,
                                         int16_t y)
{
    dgus_point_t point;

    if (!dgus_rtc_analog_has_sp(rtc)) {
        return DGUS_ERR_ARG;
    }

    point.x = x;
    point.y = y;
    return dgus_obj_desc_write_point_at(ui, &rtc->obj, rtc->sp, DGUS_RTC_ANALOG_SP_CENTER, point);
}

dgus_status_t dgus_rtc_analog_set_hand(dgus_ui_t *ui,
                                       const dgus_rtc_analog_t *rtc,
                                       uint16_t icon_offset,
                                       uint16_t center_offset,
                                       uint16_t icon_id,
                                       dgus_point_t icon_center)
{
    if (!dgus_rtc_analog_has_sp(rtc)) {
        return DGUS_ERR_ARG;
    }
    if (dgus_obj_desc_write_u16_at(ui, &rtc->obj, rtc->sp, icon_offset, icon_id) != DGUS_OK) {
        return DGUS_ERR_IO;
    }

    return dgus_obj_desc_write_point_at(ui, &rtc->obj, rtc->sp, center_offset, icon_center);
}

dgus_status_t dgus_rtc_analog_set_icon_lib(dgus_ui_t *ui,
                                           const dgus_rtc_analog_t *rtc,
                                           uint8_t icon_lib)
{
    return dgus_rtc_analog_desc_write_u16(ui,
                                          rtc,
                                          DGUS_RTC_ANALOG_SP_ICON_LIB,
                                          dgus_pack_u8(icon_lib, 0u));
}
