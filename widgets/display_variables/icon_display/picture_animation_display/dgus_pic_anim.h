#ifndef DGUS_PIC_ANIM_H
#define DGUS_PIC_ANIM_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Offsets for full-screen picture animation display (14.BIN type 0x04). */
#define DGUS_PIC_ANIM_SP_FIXED      0x00u
#define DGUS_PIC_ANIM_SP_PIC_BEGIN  0x01u
#define DGUS_PIC_ANIM_SP_PIC_END    0x02u
#define DGUS_PIC_ANIM_SP_FRAME_TIME 0x03u

/* Full-screen picture animation. This display variable has no runtime VP.
 *
 * Parameters:
 * - sp is the descriptor base for type 0x04. DGUS_SP_NONE makes all descriptor
 *   setters return DGUS_ERR_ARG.
 * - begin_pic/end_pic are inclusive DGUS picture IDs.
 * - frame_time is the manual's one-frame display-time byte.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
} dgus_pic_anim_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_pic_anim_t, dgus_pic_anim);

/* Create a picture animation object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * sp: descriptor pointer for 14.BIN type 0x04, or DGUS_SP_NONE.
 */
dgus_pic_anim_t *dgus_pic_anim_create(dgus_page_t *page,
                                      uint16_t id,
                                      uint16_t sp);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_pic_anim_has_sp(const dgus_pic_anim_t *anim);

/* Write an arbitrary descriptor word for picture animation display. */
dgus_status_t dgus_pic_anim_desc_write_u16(dgus_ui_t *ui,
                                           const dgus_pic_anim_t *anim,
                                           uint16_t offset,
                                           uint16_t value);

/* Configure the first and last picture IDs in the animation sequence. */
dgus_status_t dgus_pic_anim_set_picture_range(dgus_ui_t *ui,
                                              const dgus_pic_anim_t *anim,
                                              uint16_t begin_pic,
                                              uint16_t end_pic);

/* Configure one-frame display time in DGUS manual units. */
dgus_status_t dgus_pic_anim_set_frame_time(dgus_ui_t *ui,
                                           const dgus_pic_anim_t *anim,
                                           uint8_t frame_time);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_PIC_ANIM_H */
