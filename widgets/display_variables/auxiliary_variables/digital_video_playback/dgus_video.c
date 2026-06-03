#include "dgus_video.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_video_t, dgus_video)
dgus_video_t *dgus_video_create(dgus_page_t *page,
                                uint16_t id,
                                uint16_t vp,
                                uint16_t sp)
{
    dgus_video_t *video;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    video = (dgus_video_t *)dgus_page_alloc(page, sizeof(*video));
    if (video == NULL) {
        return NULL;
    }
    dgus_widget_init_obj(&video->obj, id, DGUS_OBJ_VIDEO, vp, 6u);
    video->sp = sp;
    if (dgus_page_add_obj(page, &video->obj) != DGUS_OK) {
        return NULL;
    }

    return video;
}

bool dgus_video_has_sp(const dgus_video_t *video)
{
    return (video != NULL) && dgus_widget_has_sp(video->sp);
}

dgus_status_t dgus_video_write_control(dgus_ui_t *ui,
                                       const dgus_video_t *video,
                                       uint8_t enable_marker,
                                       dgus_video_cmd_t command,
                                       uint16_t start_seconds)
{
    uint16_t words[2];

    if (video == NULL) {
        return DGUS_ERR_ARG;
    }

    words[0] = dgus_pack_u8(enable_marker, (uint8_t)command);
    words[1] = start_seconds;

    return dgus_obj_write_words_at(ui, &video->obj, DGUS_VIDEO_VP_CONTROL, words, 2u);
}

dgus_status_t dgus_video_play(dgus_ui_t *ui, const dgus_video_t *video)
{
    return dgus_video_write_control(ui, video, 0x5Au, DGUS_VIDEO_CMD_PLAY_FROM, 0u);
}

dgus_status_t dgus_video_stop(dgus_ui_t *ui, const dgus_video_t *video)
{
    return dgus_video_write_control(ui, video, 0x5Au, DGUS_VIDEO_CMD_STOP, 0u);
}

dgus_status_t dgus_video_pause_resume(dgus_ui_t *ui, const dgus_video_t *video)
{
    return dgus_video_write_control(ui,
                                    video,
                                    0x5Au,
                                    DGUS_VIDEO_CMD_PAUSE_RESUME,
                                    0u);
}

dgus_status_t dgus_video_play_from(dgus_ui_t *ui,
                                   const dgus_video_t *video,
                                   uint16_t seconds)
{
    return dgus_video_write_control(ui,
                                    video,
                                    0x5Au,
                                    DGUS_VIDEO_CMD_PLAY_FROM,
                                    seconds);
}

dgus_status_t dgus_video_read_feedback(dgus_ui_t *ui,
                                       const dgus_video_t *video,
                                       uint16_t offset)
{
    if ((ui == NULL) || (ui->dev == NULL) || (video == NULL)) {
        return DGUS_ERR_ARG;
    }

    return dgus_read_words(ui->dev, (uint16_t)(video->obj.vp + offset), 1u);
}

dgus_status_t dgus_video_desc_write_u16(dgus_ui_t *ui,
                                        const dgus_video_t *video,
                                        uint16_t offset,
                                        uint16_t value)
{
    if (!dgus_video_has_sp(video)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &video->obj, video->sp, offset, value);
}

dgus_status_t dgus_video_set_desc_vp(dgus_ui_t *ui,
                                     const dgus_video_t *video,
                                     uint16_t vp)
{
    return dgus_video_desc_write_u16(ui, video, DGUS_VIDEO_SP_VP, vp);
}

dgus_status_t dgus_video_hide(dgus_ui_t *ui, const dgus_video_t *video)
{
    return dgus_video_set_desc_vp(ui, video, 0xFF00u);
}

dgus_status_t dgus_video_show(dgus_ui_t *ui, const dgus_video_t *video)
{
    if (video == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_video_set_desc_vp(ui, video, video->obj.vp);
}

dgus_status_t dgus_video_set_pos(dgus_ui_t *ui,
                                 const dgus_video_t *video,
                                 int16_t x,
                                 int16_t y)
{
    dgus_point_t point;

    if (!dgus_video_has_sp(video)) {
        return DGUS_ERR_ARG;
    }

    point.x = x;
    point.y = y;
    return dgus_obj_desc_write_point_at(ui, &video->obj, video->sp, DGUS_VIDEO_SP_POS, point);
}

dgus_status_t dgus_video_set_window(dgus_ui_t *ui,
                                    const dgus_video_t *video,
                                    uint16_t width,
                                    uint16_t height)
{
    uint16_t words[2];

    words[0] = width;
    words[1] = height;
    if (!dgus_video_has_sp(video)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_words_at(ui, &video->obj, video->sp, DGUS_VIDEO_SP_WIDTH, words, 2u);
}

dgus_status_t dgus_video_set_fps_type(dgus_ui_t *ui,
                                      const dgus_video_t *video,
                                      uint8_t fps,
                                      uint8_t video_type)
{
    return dgus_video_desc_write_u16(ui,
                                     video,
                                     DGUS_VIDEO_SP_FPS_TYPE,
                                     dgus_pack_u8(fps, video_type));
}

dgus_status_t dgus_video_set_files(dgus_ui_t *ui,
                                   const dgus_video_t *video,
                                   uint16_t icl_id,
                                   uint16_t wae_id)
{
    uint16_t words[2];

    words[0] = icl_id;
    words[1] = wae_id;
    if (!dgus_video_has_sp(video)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_words_at(ui, &video->obj, video->sp, DGUS_VIDEO_SP_ICL_ID, words, 2u);
}
