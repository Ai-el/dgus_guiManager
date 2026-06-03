#ifndef DGUS_VIDEO_H
#define DGUS_VIDEO_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Offsets for digital video playback display (14.BIN type 0x31). */
#define DGUS_VIDEO_SP_VP          0x00u
#define DGUS_VIDEO_SP_POS         0x01u
#define DGUS_VIDEO_SP_WIDTH       0x03u
#define DGUS_VIDEO_SP_HEIGHT      0x04u
#define DGUS_VIDEO_SP_FPS_TYPE    0x05u
#define DGUS_VIDEO_SP_ICL_ID      0x06u
#define DGUS_VIDEO_SP_WAE_ID      0x07u

/* VP offsets for video control and feedback. */
#define DGUS_VIDEO_VP_CONTROL     0x00u
#define DGUS_VIDEO_VP_STATUS      0x02u
#define DGUS_VIDEO_VP_LENGTH      0x04u
#define DGUS_VIDEO_VP_POSITION    0x05u

typedef enum {
    DGUS_VIDEO_CMD_STOP = 1,
    DGUS_VIDEO_CMD_PAUSE_RESUME = 2,
    DGUS_VIDEO_CMD_PLAY_FROM = 3
} dgus_video_cmd_t;

/* Digital video playback display. VP controls playback and exposes feedback.
 *
 * Parameters:
 * - vp is the video control block: VP+0..1 control, VP+2 status,
 *   VP+4 length, VP+5 position.
 * - sp is the descriptor base for type 0x31, or DGUS_SP_NONE.
 * - icl_id/wae_id select the DGUS video resource files configured in the
 *   project.
 * - fps/video_type are descriptor bytes from the DGUS manual.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
} dgus_video_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_video_t, dgus_video);

/* Create a digital video display object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS video control VP block base.
 * sp: descriptor pointer for 14.BIN type 0x31, or DGUS_SP_NONE.
 */
dgus_video_t *dgus_video_create(dgus_page_t *page,
                                uint16_t id,
                                uint16_t vp,
                                uint16_t sp);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_video_has_sp(const dgus_video_t *video);

/* Write the two-word video control interface at VP and VP+1.
 * enable_marker: DGUS enable/control marker byte.
 * command: stop, pause/resume, or play-from command.
 * start_seconds: start position used by DGUS_VIDEO_CMD_PLAY_FROM.
 */
dgus_status_t dgus_video_write_control(dgus_ui_t *ui,
                                       const dgus_video_t *video,
                                       uint8_t enable_marker,
                                       dgus_video_cmd_t command,
                                       uint16_t start_seconds);

/* Start playback from the beginning. */
dgus_status_t dgus_video_play(dgus_ui_t *ui, const dgus_video_t *video);

/* Stop playback and keep the first frame on screen. */
dgus_status_t dgus_video_stop(dgus_ui_t *ui, const dgus_video_t *video);

/* Toggle pause/resume playback. */
dgus_status_t dgus_video_pause_resume(dgus_ui_t *ui, const dgus_video_t *video);

/* Start playback from a specific position in seconds. */
dgus_status_t dgus_video_play_from(dgus_ui_t *ui,
                                   const dgus_video_t *video,
                                   uint16_t seconds);

/* Request one feedback word such as VP+2 status, VP+4 length, or VP+5 position.
 * offset: use DGUS_VIDEO_VP_STATUS, DGUS_VIDEO_VP_LENGTH, or
 *   DGUS_VIDEO_VP_POSITION.
 */
dgus_status_t dgus_video_read_feedback(dgus_ui_t *ui,
                                       const dgus_video_t *video,
                                       uint16_t offset);

/* Write an arbitrary descriptor word for video display. */
dgus_status_t dgus_video_desc_write_u16(dgus_ui_t *ui,
                                        const dgus_video_t *video,
                                        uint16_t offset,
                                        uint16_t value);

/* Change the linked VP at SP+0. 0xFF00 hides the display. */
dgus_status_t dgus_video_set_desc_vp(dgus_ui_t *ui,
                                     const dgus_video_t *video,
                                     uint16_t vp);

/* Hide by writing 0xFF00 to SP+0. */
dgus_status_t dgus_video_hide(dgus_ui_t *ui, const dgus_video_t *video);

/* Show by restoring a linked VP at SP+0. */
dgus_status_t dgus_video_show(dgus_ui_t *ui, const dgus_video_t *video);

/* Move the video window by writing X,Y to SP+1. */
dgus_status_t dgus_video_set_pos(dgus_ui_t *ui,
                                 const dgus_video_t *video,
                                 int16_t x,
                                 int16_t y);

/* Set video window width and height. */
dgus_status_t dgus_video_set_window(dgus_ui_t *ui,
                                    const dgus_video_t *video,
                                    uint16_t width,
                                    uint16_t height);

/* Configure frame rate and video type. */
dgus_status_t dgus_video_set_fps_type(dgus_ui_t *ui,
                                      const dgus_video_t *video,
                                      uint8_t fps,
                                      uint8_t video_type);

/* Configure ICL and WAE file IDs. */
dgus_status_t dgus_video_set_files(dgus_ui_t *ui,
                                   const dgus_video_t *video,
                                   uint16_t icl_id,
                                   uint16_t wae_id);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_VIDEO_H */
