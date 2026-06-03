#ifndef DGUS_GTF_TEXT_H
#define DGUS_GTF_TEXT_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Offsets for GTF icon-font text display (14.BIN type 0x18). */
#define DGUS_GTF_TEXT_SP_VP             0x00u
#define DGUS_GTF_TEXT_SP_GTF_ID         0x01u
#define DGUS_GTF_TEXT_SP_FONT_ID        0x02u
#define DGUS_GTF_TEXT_SP_POS            0x03u
#define DGUS_GTF_TEXT_SP_BOX            0x05u
#define DGUS_GTF_TEXT_SP_SPACING        0x09u
#define DGUS_GTF_TEXT_SP_DISPLAY_MODE   0x0Au

/* GTF icon-font text display.
 *
 * Parameters:
 * - vp stores the text content.
 * - sp is the descriptor base for type 0x18, or DGUS_SP_NONE.
 * - gtf_id/font_id select the icon-font resources in the DGUS project.
 * - display_mode is the manual descriptor mode byte.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
    uint8_t max_chars;
    bool terminate;
} dgus_gtf_text_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_gtf_text_t, dgus_gtf_text);

/* Create a GTF text display object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS text VP base.
 * sp: descriptor pointer for 14.BIN type 0x18, or DGUS_SP_NONE.
 * max_chars: maximum ASCII characters written to VP.
 */
dgus_gtf_text_t *dgus_gtf_text_create(dgus_page_t *page,
                                      uint16_t id,
                                      uint16_t vp,
                                      uint16_t sp,
                                      uint8_t max_chars);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_gtf_text_has_sp(const dgus_gtf_text_t *text);

/* Configure whether text writes append the DGUS 0xFFFF terminator. */
void dgus_gtf_text_set_terminate(dgus_gtf_text_t *text, bool terminate);

/* Write ASCII text to the object's VP. */
dgus_status_t dgus_gtf_text_set_text_ascii(dgus_ui_t *ui,
                                           const dgus_gtf_text_t *text,
                                           const char *value);

/* Write an arbitrary descriptor word for GTF text display. */
dgus_status_t dgus_gtf_text_desc_write_u16(dgus_ui_t *ui,
                                           const dgus_gtf_text_t *text,
                                           uint16_t offset,
                                           uint16_t value);

/* Change the linked VP at SP+0. 0xFF00 hides the display. */
dgus_status_t dgus_gtf_text_set_desc_vp(dgus_ui_t *ui,
                                        const dgus_gtf_text_t *text,
                                        uint16_t vp);

/* Hide by writing 0xFF00 to SP+0. */
dgus_status_t dgus_gtf_text_hide(dgus_ui_t *ui,
                                 const dgus_gtf_text_t *text);

/* Show by restoring a linked VP at SP+0. */
dgus_status_t dgus_gtf_text_show(dgus_ui_t *ui,
                                 const dgus_gtf_text_t *text);

/* Configure GTF and font IDs. */
dgus_status_t dgus_gtf_text_set_ids(dgus_ui_t *ui,
                                    const dgus_gtf_text_t *text,
                                    uint16_t gtf_id,
                                    uint16_t font_id);

/* Move the display by writing X,Y to SP+3. */
dgus_status_t dgus_gtf_text_set_pos(dgus_ui_t *ui,
                                    const dgus_gtf_text_t *text,
                                    int16_t x,
                                    int16_t y);

/* Change text display box at SP+5. */
dgus_status_t dgus_gtf_text_set_box(dgus_ui_t *ui,
                                    const dgus_gtf_text_t *text,
                                    dgus_rect_t box);

/* Configure horizontal and vertical icon spacing at SP+9. */
dgus_status_t dgus_gtf_text_set_spacing(dgus_ui_t *ui,
                                        const dgus_gtf_text_t *text,
                                        uint8_t horizontal,
                                        uint8_t vertical);

/* Configure GTF display mode bits at SP+0x0A. */
dgus_status_t dgus_gtf_text_set_display_mode(dgus_ui_t *ui,
                                             const dgus_gtf_text_t *text,
                                             uint8_t display_mode);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_GTF_TEXT_H */
