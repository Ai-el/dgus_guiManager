#include "example_widgets.h"

#define EXAMPLE_PIC_ANIM_ID 7u
#define EXAMPLE_PIC_ANIM_SP 0x8080u

dgus_pic_anim_t *dgus_example_pic_anim_create(dgus_example_ctx_t *ctx)
{
    dgus_pic_anim_t *anim;

    if (ctx == NULL) {
        return NULL;
    }

    anim = dgus_pic_anim_create(&ctx->page,
                                EXAMPLE_PIC_ANIM_ID,
                                EXAMPLE_PIC_ANIM_SP);
    if (anim == NULL) {
        return NULL;
    }

    (void)dgus_pic_anim_set_picture_range(&ctx->ui, anim, 10u, 16u);
    (void)dgus_pic_anim_set_frame_time(&ctx->ui, anim, 6u);

    return anim;
}
