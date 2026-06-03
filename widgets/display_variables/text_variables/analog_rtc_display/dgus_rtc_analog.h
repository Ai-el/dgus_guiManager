#ifndef DGUS_RTC_ANALOG_H
#define DGUS_RTC_ANALOG_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Offsets for analog RTC display (14.BIN type 0x12_01). */
#define DGUS_RTC_ANALOG_SP_FIXED          0x00u
#define DGUS_RTC_ANALOG_SP_CENTER         0x01u
#define DGUS_RTC_ANALOG_SP_HOUR_ICON      0x03u
#define DGUS_RTC_ANALOG_SP_HOUR_CENTER    0x04u
#define DGUS_RTC_ANALOG_SP_MINUTE_ICON    0x06u
#define DGUS_RTC_ANALOG_SP_MINUTE_CENTER  0x07u
#define DGUS_RTC_ANALOG_SP_SECOND_ICON    0x09u
#define DGUS_RTC_ANALOG_SP_SECOND_CENTER  0x0Au
#define DGUS_RTC_ANALOG_SP_ICON_LIB       0x0Cu

/* Analog RTC display. It reads the screen RTC and has no runtime VP.
 *
 * Parameters:
 * - sp is the descriptor base for type 0x12 analog RTC, or DGUS_SP_NONE.
 * - icon_offset/center_offset should be one of the DGUS_RTC_ANALOG_SP_* hand
 *   offset pairs, for example HOUR_ICON and HOUR_CENTER.
 * - icon_center is the rotation center inside the hand icon bitmap.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
} dgus_rtc_analog_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_rtc_analog_t, dgus_rtc_analog);

/* Create an analog RTC display object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * sp: descriptor pointer for 14.BIN type 0x12 analog RTC, or DGUS_SP_NONE.
 */
dgus_rtc_analog_t *dgus_rtc_analog_create(dgus_page_t *page,
                                          uint16_t id,
                                          uint16_t sp);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_rtc_analog_has_sp(const dgus_rtc_analog_t *rtc);

/* Write an arbitrary descriptor word for analog RTC display. */
dgus_status_t dgus_rtc_analog_desc_write_u16(dgus_ui_t *ui,
                                             const dgus_rtc_analog_t *rtc,
                                             uint16_t offset,
                                             uint16_t value);

/* Set the clock dial center at SP+1. */
dgus_status_t dgus_rtc_analog_set_center(dgus_ui_t *ui,
                                         const dgus_rtc_analog_t *rtc,
                                         int16_t x,
                                         int16_t y);

/* Configure one hand icon and its rotation center.
 * icon_offset: descriptor offset for icon id, such as
 *   DGUS_RTC_ANALOG_SP_HOUR_ICON.
 * center_offset: descriptor offset for center point, such as
 *   DGUS_RTC_ANALOG_SP_HOUR_CENTER.
 */
dgus_status_t dgus_rtc_analog_set_hand(dgus_ui_t *ui,
                                       const dgus_rtc_analog_t *rtc,
                                       uint16_t icon_offset,
                                       uint16_t center_offset,
                                       uint16_t icon_id,
                                       dgus_point_t icon_center);

/* Set the icon library id used by clock hands. */
dgus_status_t dgus_rtc_analog_set_icon_lib(dgus_ui_t *ui,
                                           const dgus_rtc_analog_t *rtc,
                                           uint8_t icon_lib);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_RTC_ANALOG_H */
