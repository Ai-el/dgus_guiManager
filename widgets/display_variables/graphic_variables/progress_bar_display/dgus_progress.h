#ifndef DGUS_PROGRESS_H
#define DGUS_PROGRESS_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Offsets for progress bar display (14.BIN type 0x23). */
#define DGUS_PROGRESS_SP_VP          0x00u
#define DGUS_PROGRESS_SP_START       0x01u
#define DGUS_PROGRESS_SP_END         0x03u
#define DGUS_PROGRESS_SP_BORDER      0x05u
#define DGUS_PROGRESS_SP_FOREGROUND  0x06u
#define DGUS_PROGRESS_SP_BACKGROUND  0x07u
#define DGUS_PROGRESS_SP_MAX         0x08u
#define DGUS_PROGRESS_SP_MIN         0x09u
#define DGUS_PROGRESS_SP_MODE_DIR    0x0Au
#define DGUS_PROGRESS_SP_DATA_MODE   0x0Bu
#define DGUS_PROGRESS_SP_RETURN_VP   0x0Cu

/* Progress frame/background drawing mode. */
typedef enum {
    DGUS_PROGRESS_FRAME_FILL_BG = 0,
    DGUS_PROGRESS_NO_FRAME_FILL_BG = 1,
    DGUS_PROGRESS_FRAME_NO_BG = 2,
    DGUS_PROGRESS_NO_FRAME_NO_BG = 3
} dgus_progress_frame_mode_t;

/* Progress bar display.
 *
 * Parameters:
 * - vp stores the progress source value.
 * - sp is the descriptor base for type 0x23, or DGUS_SP_NONE.
 * - min_value/max_value in set_range() are MCU-side metadata; set_value_limits()
 *   writes the DGUS descriptor limits.
 * - return_vp is optional and receives percentage feedback when enabled by
 *   return_percent.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
    int32_t min_value;
    int32_t max_value;
} dgus_progress_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_progress_t, dgus_progress);

typedef dgus_progress_t dgus_bar_t;
DGUS_WIDGET_DECLARE_CALLBACKS(dgus_bar_t, dgus_bar);

/* Create a progress bar display object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS VP storing the progress value.
 * sp: descriptor pointer for 14.BIN type 0x23, or DGUS_SP_NONE.
 */
dgus_progress_t *dgus_progress_create(dgus_page_t *page,
                                      uint16_t id,
                                      uint16_t vp,
                                      uint16_t sp);

/* Backward-compatible alias for a progress bar. */
dgus_bar_t *dgus_bar_create(dgus_page_t *page,
                            uint16_t id,
                            uint16_t vp,
                            uint16_t sp);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_progress_has_sp(const dgus_progress_t *bar);

/* Set numeric range metadata stored on the MCU side. */
void dgus_progress_set_range(dgus_progress_t *bar,
                             int32_t min_value,
                             int32_t max_value);
void dgus_bar_set_range(dgus_bar_t *bar,
                        int32_t min_value,
                        int32_t max_value);

/* Write the progress value to the object's VP. */
dgus_status_t dgus_progress_set_value(dgus_ui_t *ui,
                                      const dgus_progress_t *bar,
                                      uint16_t value);
dgus_status_t dgus_bar_set_value(dgus_ui_t *ui,
                                 const dgus_bar_t *bar,
                                 uint16_t value);

/* Write arbitrary descriptor fields for progress display. */
dgus_status_t dgus_progress_desc_write_u16(dgus_ui_t *ui,
                                           const dgus_progress_t *bar,
                                           uint16_t offset,
                                           uint16_t value);
dgus_status_t dgus_progress_desc_write_words(dgus_ui_t *ui,
                                             const dgus_progress_t *bar,
                                             uint16_t offset,
                                             const uint16_t *words,
                                             uint8_t word_count);

/* Set progress rectangle at SP+1..SP+4. */
dgus_status_t dgus_progress_set_area(dgus_ui_t *ui,
                                     const dgus_progress_t *bar,
                                     dgus_rect_t area);

/* Configure border, foreground, and background colors. */
dgus_status_t dgus_progress_set_colors(dgus_ui_t *ui,
                                       const dgus_progress_t *bar,
                                       uint16_t border,
                                       uint16_t foreground,
                                       uint16_t background);

/* Configure min/max values used by the progress display. */
dgus_status_t dgus_progress_set_value_limits(dgus_ui_t *ui,
                                             const dgus_progress_t *bar,
                                             int16_t min_value,
                                             int16_t max_value);

/* Configure frame/background mode, direction, and return-percent flag.
 * frame: border/background drawing mode.
 * dir: fill direction, one of dgus_progress_dir_t.
 * return_percent: true asks DGUS to write percentage to return_vp.
 */
dgus_status_t dgus_progress_set_mode(dgus_ui_t *ui,
                                     const dgus_progress_t *bar,
                                     dgus_progress_frame_mode_t frame,
                                     dgus_progress_dir_t dir,
                                     bool return_percent);

/* Configure optional return VP at SP+0x0C. */
dgus_status_t dgus_progress_set_return_vp(dgus_ui_t *ui,
                                          const dgus_progress_t *bar,
                                          uint16_t return_vp);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_PROGRESS_H */
