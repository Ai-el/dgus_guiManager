#include "dgus_pic_anim.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_pic_anim_t, dgus_pic_anim)
dgus_pic_anim_t *dgus_pic_anim_create(dgus_page_t *page,
                                      uint16_t id,
                                      uint16_t sp)
{
    dgus_pic_anim_t *anim;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    anim = (dgus_pic_anim_t *)dgus_page_alloc(page, sizeof(*anim));
    if (anim == NULL) {
        return NULL;
    }
    dgus_widget_init_obj(&anim->obj, id, DGUS_OBJ_PIC_ANIM, DGUS_VP_NONE, 0u);
    anim->sp = sp;
    if (dgus_page_add_obj(page, &anim->obj) != DGUS_OK) {
        return NULL;
    }

    return anim;
}

bool dgus_pic_anim_has_sp(const dgus_pic_anim_t *anim)
{
    return (anim != NULL) && dgus_widget_has_sp(anim->sp);
}

dgus_status_t dgus_pic_anim_desc_write_u16(dgus_ui_t *ui,
                                           const dgus_pic_anim_t *anim,
                                           uint16_t offset,
                                           uint16_t value)
{
    if (!dgus_pic_anim_has_sp(anim)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &anim->obj, anim->sp, offset, value);
}

dgus_status_t dgus_pic_anim_set_picture_range(dgus_ui_t *ui,
                                              const dgus_pic_anim_t *anim,
                                              uint16_t begin_pic,
                                              uint16_t end_pic)
{
    uint16_t words[2];

    words[0] = begin_pic;
    words[1] = end_pic;
    if (!dgus_pic_anim_has_sp(anim)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_words_at(ui, &anim->obj, anim->sp, DGUS_PIC_ANIM_SP_PIC_BEGIN, words, 2u);
}

dgus_status_t dgus_pic_anim_set_frame_time(dgus_ui_t *ui,
                                           const dgus_pic_anim_t *anim,
                                           uint8_t frame_time)
{
    return dgus_pic_anim_desc_write_u16(ui,
                                        anim,
                                        DGUS_PIC_ANIM_SP_FRAME_TIME,
                                        dgus_pack_u8(frame_time, 0u));
}
