#ifndef DGUS_RTC_TEXT_H
#define DGUS_RTC_TEXT_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Offsets for text RTC display (14.BIN type 0x12_00). */
#define DGUS_RTC_TEXT_SP_FIXED      0x00u
#define DGUS_RTC_TEXT_SP_POS        0x01u
#define DGUS_RTC_TEXT_SP_COLOR      0x03u
#define DGUS_RTC_TEXT_SP_FONT       0x04u
#define DGUS_RTC_TEXT_SP_FORMAT     0x05u
#define DGUS_RTC_TEXT_FORMAT_MAX    16u

/* Text RTC display. It reads the screen RTC and has no runtime VP.
 *
 * Parameters:
 * - sp is the descriptor base for type 0x12 text RTC, or DGUS_SP_NONE.
 * - font_lib and x_dots select the DGUS font used for RTC text.
 * - format is the DGUS RTC format string, limited by
 *   DGUS_RTC_TEXT_FORMAT_MAX.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
} dgus_rtc_text_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_rtc_text_t, dgus_rtc_text);

/* Create a text RTC display object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * sp: descriptor pointer for 14.BIN type 0x12 text RTC, or DGUS_SP_NONE.
 */
dgus_rtc_text_t *dgus_rtc_text_create(dgus_page_t *page,
                                      uint16_t id,
                                      uint16_t sp);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_rtc_text_has_sp(const dgus_rtc_text_t *rtc);

/* Write an arbitrary descriptor word for text RTC display. */
dgus_status_t dgus_rtc_text_desc_write_u16(dgus_ui_t *ui,
                                           const dgus_rtc_text_t *rtc,
                                           uint16_t offset,
                                           uint16_t value);

/* Move the RTC text by writing X,Y to SP+1. */
dgus_status_t dgus_rtc_text_set_pos(dgus_ui_t *ui,
                                    const dgus_rtc_text_t *rtc,
                                    int16_t x,
                                    int16_t y);

/* Change the RTC text color at SP+3. */
dgus_status_t dgus_rtc_text_set_color(dgus_ui_t *ui,
                                      const dgus_rtc_text_t *rtc,
                                      uint16_t rgb565);

/* Set font library and X dot size at SP+4. */
dgus_status_t dgus_rtc_text_set_font(dgus_ui_t *ui,
                                     const dgus_rtc_text_t *rtc,
                                     uint8_t font_lib,
                                     uint8_t x_dots);

/* Write an RTC format string such as "Y-M-D H:Q:S" to SP+5.
 * format: DGUS RTC format characters; the helper pads/truncates to the
 * descriptor field length.
 */
dgus_status_t dgus_rtc_text_set_format(dgus_ui_t *ui,
                                       const dgus_rtc_text_t *rtc,
                                       const char *format);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_RTC_TEXT_H */
