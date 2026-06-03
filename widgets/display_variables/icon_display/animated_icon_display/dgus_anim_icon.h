#ifndef DGUS_ANIM_ICON_H
#define DGUS_ANIM_ICON_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Offsets for animated icon display (14.BIN type 0x01). */
#define DGUS_ANIM_ICON_SP_VP          0x00u
#define DGUS_ANIM_ICON_SP_POS         0x01u
#define DGUS_ANIM_ICON_SP_RESET       0x03u
#define DGUS_ANIM_ICON_SP_V_STOP      0x04u
#define DGUS_ANIM_ICON_SP_V_START     0x05u
#define DGUS_ANIM_ICON_SP_ICON_STOP   0x06u
#define DGUS_ANIM_ICON_SP_ICON_START  0x07u
#define DGUS_ANIM_ICON_SP_ICON_END    0x08u
#define DGUS_ANIM_ICON_SP_LIB_MODE    0x09u
#define DGUS_ANIM_ICON_SP_LAYER       0x0Au
#define DGUS_ANIM_ICON_SP_GAMMA_TIME  0x0Bu
#define DGUS_ANIM_ICON_SP_PLAY_FILTER 0x0Cu

/* Animated icon playback behavior stored in the descriptor timing field. */
typedef enum {
    DGUS_ANIM_ICON_LOOP = 0,
    DGUS_ANIM_ICON_ONCE = 1
} dgus_anim_icon_play_mode_t;

/* Animated icon display. The VP value controls stop/start animation states.
 *
 * Parameters:
 * - vp stores the animation state value. stop_value shows stop_icon;
 *   start_value starts animation from start_icon to end_icon.
 * - sp is the descriptor base for type 0x01, or DGUS_SP_NONE for VP-only use.
 * - frame_time is the manual's per-frame time byte; play_mode selects loop or
 *   once.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
} dgus_anim_icon_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_anim_icon_t, dgus_anim_icon);

/* Create an animated icon display object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS VP storing stop/start state.
 * sp: descriptor pointer for 14.BIN type 0x01, or DGUS_SP_NONE.
 */
dgus_anim_icon_t *dgus_anim_icon_create(dgus_page_t *page,
                                        uint16_t id,
                                        uint16_t vp,
                                        uint16_t sp);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_anim_icon_has_sp(const dgus_anim_icon_t *icon);

/* Write the VP state value.
 * value: usually stop_value or start_value configured by set_values().
 */
dgus_status_t dgus_anim_icon_set_value(dgus_ui_t *ui,
                                       const dgus_anim_icon_t *icon,
                                       uint16_t value);

/* Write arbitrary descriptor fields for animated icon display.
 * offset: word offset from sp; prefer DGUS_ANIM_ICON_SP_* constants.
 */
dgus_status_t dgus_anim_icon_desc_write_u16(dgus_ui_t *ui,
                                            const dgus_anim_icon_t *icon,
                                            uint16_t offset,
                                            uint16_t value);
dgus_status_t dgus_anim_icon_desc_write_words(dgus_ui_t *ui,
                                              const dgus_anim_icon_t *icon,
                                              uint16_t offset,
                                              const uint16_t *words,
                                              uint8_t word_count);

/* Hide/show by writing the linked VP field at SP+0. */
dgus_status_t dgus_anim_icon_set_desc_vp(dgus_ui_t *ui,
                                         const dgus_anim_icon_t *icon,
                                         uint16_t vp);
dgus_status_t dgus_anim_icon_hide(dgus_ui_t *ui,
                                  const dgus_anim_icon_t *icon);
dgus_status_t dgus_anim_icon_show(dgus_ui_t *ui,
                                  const dgus_anim_icon_t *icon);

/* Move by writing X,Y to SP+1.
 * x/y: top-left icon position in pixels.
 */
dgus_status_t dgus_anim_icon_set_pos(dgus_ui_t *ui,
                                     const dgus_anim_icon_t *icon,
                                     int16_t x,
                                     int16_t y);

/* Configure stop/start VP values.
 * stop_value: VP word that stops animation.
 * start_value: VP word that starts animation.
 */
dgus_status_t dgus_anim_icon_set_values(dgus_ui_t *ui,
                                        const dgus_anim_icon_t *icon,
                                        uint16_t stop_value,
                                        uint16_t start_value);
/* Configure stop/start/end icon IDs from the selected icon library. */
dgus_status_t dgus_anim_icon_set_icon_range(dgus_ui_t *ui,
                                            const dgus_anim_icon_t *icon,
                                            uint16_t stop_icon,
                                            uint16_t start_icon,
                                            uint16_t end_icon);

/* Configure per-frame time and loop/once mode.
 * frame_time: DGUS frame interval byte from the manual.
 * play_mode: DGUS_ANIM_ICON_LOOP or DGUS_ANIM_ICON_ONCE.
 */
dgus_status_t dgus_anim_icon_set_timing(dgus_ui_t *ui,
                                        const dgus_anim_icon_t *icon,
                                        uint8_t frame_time,
                                        dgus_anim_icon_play_mode_t play_mode);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_ANIM_ICON_H */
