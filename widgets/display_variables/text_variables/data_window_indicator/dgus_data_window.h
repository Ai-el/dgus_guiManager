#ifndef DGUS_DATA_WINDOW_H
#define DGUS_DATA_WINDOW_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Offsets for data window indicator display (14.BIN type 0x15). */
#define DGUS_DATA_WINDOW_SP_VP              0x00u
#define DGUS_DATA_WINDOW_SP_MIN             0x01u
#define DGUS_DATA_WINDOW_SP_MAX             0x02u
#define DGUS_DATA_WINDOW_SP_DIGITS          0x03u
#define DGUS_DATA_WINDOW_SP_COUNT_MODE      0x04u
#define DGUS_DATA_WINDOW_SP_CENTER          0x05u
#define DGUS_DATA_WINDOW_SP_STEP            0x07u
#define DGUS_DATA_WINDOW_SP_NORMAL_DOTS     0x08u
#define DGUS_DATA_WINDOW_SP_NORMAL_COLOR    0x09u
#define DGUS_DATA_WINDOW_SP_SELECTED_DOTS   0x0Au
#define DGUS_DATA_WINDOW_SP_SELECTED_COLOR  0x0Bu

/* Data window indicator. VP is a double-word control; VP+1 is reserved.
 *
 * Parameters:
 * - vp stores the selected value at VP+0.
 * - sp is the descriptor base for type 0x15, or DGUS_SP_NONE.
 * - count is the number of visible rows/items, mode is the manual display-mode
 *   byte, and step is the adjustment increment.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
} dgus_data_window_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_data_window_t, dgus_data_window);

/* Create a data window indicator object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS VP storing the selected value.
 * sp: descriptor pointer for 14.BIN type 0x15, or DGUS_SP_NONE.
 */
dgus_data_window_t *dgus_data_window_create(dgus_page_t *page,
                                            uint16_t id,
                                            uint16_t vp,
                                            uint16_t sp);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_data_window_has_sp(const dgus_data_window_t *window);

/* Write the selected value to the object's VP. */
dgus_status_t dgus_data_window_set_value(dgus_ui_t *ui,
                                         const dgus_data_window_t *window,
                                         int16_t value);

/* Write an arbitrary descriptor word for data window display. */
dgus_status_t dgus_data_window_desc_write_u16(dgus_ui_t *ui,
                                              const dgus_data_window_t *window,
                                              uint16_t offset,
                                              uint16_t value);

/* Change the linked VP at SP+0. 0xFF00 hides the display. */
dgus_status_t dgus_data_window_set_desc_vp(dgus_ui_t *ui,
                                           const dgus_data_window_t *window,
                                           uint16_t vp);

/* Hide by writing 0xFF00 to SP+0. */
dgus_status_t dgus_data_window_hide(dgus_ui_t *ui,
                                    const dgus_data_window_t *window);

/* Show by restoring a linked VP at SP+0. */
dgus_status_t dgus_data_window_show(dgus_ui_t *ui,
                                    const dgus_data_window_t *window);

/* Configure data min and max descriptor fields. */
dgus_status_t dgus_data_window_set_range(dgus_ui_t *ui,
                                         const dgus_data_window_t *window,
                                         int16_t min_value,
                                         int16_t max_value);

/* Configure integer and decimal digit counts. */
dgus_status_t dgus_data_window_set_digits(dgus_ui_t *ui,
                                          const dgus_data_window_t *window,
                                          uint8_t integer_digits,
                                          uint8_t decimal_digits);

/* Configure total visible count and display mode bits. */
dgus_status_t dgus_data_window_set_count_mode(dgus_ui_t *ui,
                                              const dgus_data_window_t *window,
                                              uint8_t count,
                                              uint8_t mode);

/* Set the window center point. */
dgus_status_t dgus_data_window_set_center(dgus_ui_t *ui,
                                          const dgus_data_window_t *window,
                                          int16_t x,
                                          int16_t y);

/* Configure adjustment step. */
dgus_status_t dgus_data_window_set_step(dgus_ui_t *ui,
                                        const dgus_data_window_t *window,
                                        uint16_t step);

/* Configure unselected/selected font dots and colors.
 * normal_*: font size/color for non-selected rows.
 * selected_*: font size/color for the selected row.
 */
dgus_status_t dgus_data_window_set_text_style(dgus_ui_t *ui,
                                              const dgus_data_window_t *window,
                                              uint8_t normal_x_dots,
                                              uint8_t normal_y_dots,
                                              uint16_t normal_color,
                                              uint8_t selected_x_dots,
                                              uint8_t selected_y_dots,
                                              uint16_t selected_color);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_DATA_WINDOW_H */
