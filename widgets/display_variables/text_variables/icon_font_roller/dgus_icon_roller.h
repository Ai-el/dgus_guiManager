#ifndef DGUS_ICON_ROLLER_H
#define DGUS_ICON_ROLLER_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Offsets for icon-font roller display (14.BIN type 0x17). */
#define DGUS_ICON_ROLLER_SP_VP             0x00u
#define DGUS_ICON_ROLLER_SP_ADJ_DATA       0x01u
#define DGUS_ICON_ROLLER_SP_STRING_VP      0x02u
#define DGUS_ICON_ROLLER_SP_V_MIN          0x03u
#define DGUS_ICON_ROLLER_SP_V_MAX          0x04u
#define DGUS_ICON_ROLLER_SP_MODE_SPEED     0x05u
#define DGUS_ICON_ROLLER_SP_FONT_ID        0x06u
#define DGUS_ICON_ROLLER_SP_SELECTED_POS   0x07u
#define DGUS_ICON_ROLLER_SP_LINE_HEIGHTS_0 0x09u
#define DGUS_ICON_ROLLER_SP_DIM_HEIGHT     0x0Bu
#define DGUS_ICON_ROLLER_SP_FONT_LINES     0x0Cu

/* Icon-font roller display. VP stores selected value and adjustment fields.
 *
 * Parameters:
 * - vp stores the selected value.
 * - sp is the descriptor base for type 0x17, or DGUS_SP_NONE.
 * - adjust_mode/data_mode/display_mode/speed are DGUS manual mode bytes.
 * - value at SP+2 is either numeric step or a string VP, depending on mode.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
} dgus_icon_roller_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_icon_roller_t, dgus_icon_roller);

/* Create an icon-font roller display object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS VP storing the selected roller value.
 * sp: descriptor pointer for 14.BIN type 0x17, or DGUS_SP_NONE.
 */
dgus_icon_roller_t *dgus_icon_roller_create(dgus_page_t *page,
                                            uint16_t id,
                                            uint16_t vp,
                                            uint16_t sp);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_icon_roller_has_sp(const dgus_icon_roller_t *roller);

/* Write the selected value to the object's VP. */
dgus_status_t dgus_icon_roller_set_value(dgus_ui_t *ui,
                                         const dgus_icon_roller_t *roller,
                                         int16_t value);

/* Write an arbitrary descriptor word for icon-font roller display. */
dgus_status_t dgus_icon_roller_desc_write_u16(dgus_ui_t *ui,
                                              const dgus_icon_roller_t *roller,
                                              uint16_t offset,
                                              uint16_t value);

/* Change the linked VP at SP+0. 0xFF00 hides the display. */
dgus_status_t dgus_icon_roller_set_desc_vp(dgus_ui_t *ui,
                                           const dgus_icon_roller_t *roller,
                                           uint16_t vp);

/* Hide by writing 0xFF00 to SP+0. */
dgus_status_t dgus_icon_roller_hide(dgus_ui_t *ui,
                                    const dgus_icon_roller_t *roller);

/* Show by restoring a linked VP at SP+0. */
dgus_status_t dgus_icon_roller_show(dgus_ui_t *ui,
                                    const dgus_icon_roller_t *roller);

/* Configure adjustment mode and data mode at SP+1. */
dgus_status_t dgus_icon_roller_set_adjust_data_mode(dgus_ui_t *ui,
                                                    const dgus_icon_roller_t *roller,
                                                    uint8_t adjust_mode,
                                                    uint8_t data_mode);

/* Configure numeric/string step pointer at SP+2. */
dgus_status_t dgus_icon_roller_set_step_or_string_vp(dgus_ui_t *ui,
                                                     const dgus_icon_roller_t *roller,
                                                     uint16_t value);

/* Configure value min and max descriptor fields. */
dgus_status_t dgus_icon_roller_set_range(dgus_ui_t *ui,
                                         const dgus_icon_roller_t *roller,
                                         int16_t min_value,
                                         int16_t max_value);

/* Configure display mode and speed at SP+5. */
dgus_status_t dgus_icon_roller_set_mode_speed(dgus_ui_t *ui,
                                              const dgus_icon_roller_t *roller,
                                              uint8_t display_mode,
                                              uint8_t speed);

/* Configure icon-font id at SP+6. */
dgus_status_t dgus_icon_roller_set_font_id(dgus_ui_t *ui,
                                           const dgus_icon_roller_t *roller,
                                           uint16_t font_id);

/* Set selected row display coordinate at SP+7. */
dgus_status_t dgus_icon_roller_set_selected_pos(dgus_ui_t *ui,
                                                const dgus_icon_roller_t *roller,
                                                int16_t x,
                                                int16_t y);

/* Configure four row-gap bytes stored at SP+9 and SP+0x0A. */
dgus_status_t dgus_icon_roller_set_line_heights(dgus_ui_t *ui,
                                                const dgus_icon_roller_t *roller,
                                                uint8_t h0,
                                                uint8_t h1,
                                                uint8_t h2,
                                                uint8_t h3);

/* Configure unselected brightness and selected-row height at SP+0x0B. */
dgus_status_t dgus_icon_roller_set_brightness_height(dgus_ui_t *ui,
                                                     const dgus_icon_roller_t *roller,
                                                     uint8_t dim_no_select,
                                                     uint8_t selected_height);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_ICON_ROLLER_H */
