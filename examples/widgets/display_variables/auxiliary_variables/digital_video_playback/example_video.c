#include "example_widgets.h"

#define EXAMPLE_VIDEO_ID 30u
#define EXAMPLE_VIDEO_VP 0x1410u
#define EXAMPLE_VIDEO_SP 0x8520u

dgus_video_t *dgus_example_video_create(dgus_example_ctx_t *ctx)
{
    dgus_video_t *video;

    if (ctx == NULL) {
        return NULL;
    }

    video = dgus_video_create(&ctx->page,
                              EXAMPLE_VIDEO_ID,
                              EXAMPLE_VIDEO_VP,
                              EXAMPLE_VIDEO_SP);
    if (video == NULL) {
        return NULL;
    }

    (void)dgus_video_set_pos(&ctx->ui, video, 80, 480);
    (void)dgus_video_set_window(&ctx->ui, video, 320u, 180u);
    (void)dgus_video_set_fps_type(&ctx->ui, video, 25u, 0u);
    (void)dgus_video_set_files(&ctx->ui, video, 1u, 1u);
    (void)dgus_video_play(&ctx->ui, video);
    (void)dgus_video_play_from(&ctx->ui, video, 10u);
    (void)dgus_video_read_feedback(&ctx->ui, video, DGUS_VIDEO_VP_STATUS);

    return video;
}
