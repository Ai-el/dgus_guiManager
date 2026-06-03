#include "dgus_icon_rotate.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_icon_rotate_t, dgus_icon_rotate)
dgus_icon_rotate_t *dgus_icon_rotate_create(dgus_page_t *page,
                                            uint16_t id,
                                            uint16_t vp,
                                            uint16_t sp)
{
    dgus_icon_rotate_t *rot;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    rot = (dgus_icon_rotate_t *)dgus_page_alloc(page, sizeof(*rot));
    if (rot == NULL) {
        return NULL;
    }
    dgus_widget_init_obj(&rot->obj, id, DGUS_OBJ_ICON_ROTATE, vp, 1u);
    rot->sp = sp;
    if (dgus_page_add_obj(page, &rot->obj) != DGUS_OK) {
        return NULL;
    }

    return rot;
}

bool dgus_icon_rotate_has_sp(const dgus_icon_rotate_t *rot)
{
    return (rot != NULL) && dgus_widget_has_sp(rot->sp);
}

dgus_status_t dgus_icon_rotate_set_value(dgus_ui_t *ui,
                                         const dgus_icon_rotate_t *rot,
                                         uint16_t value)
{
    if (rot == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_set_u16(ui, &rot->obj, value);
}

dgus_status_t dgus_icon_rotate_desc_write_u16(dgus_ui_t *ui,
                                              const dgus_icon_rotate_t *rot,
                                              uint16_t offset,
                                              uint16_t value)
{
    if (!dgus_icon_rotate_has_sp(rot)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &rot->obj, rot->sp, offset, value);
}

dgus_status_t dgus_icon_rotate_set_icon(dgus_ui_t *ui,
                                        const dgus_icon_rotate_t *rot,
                                        uint16_t icon_id,
                                        uint8_t icon_lib)
{
    dgus_status_t status;

    status = dgus_icon_rotate_desc_write_u16(ui,
                                             rot,
                                             DGUS_ICON_ROTATE_SP_ICON_ID,
                                             icon_id);
    if (status != DGUS_OK) {
        return status;
    }

    return dgus_icon_rotate_desc_write_u16(ui,
                                           rot,
                                           DGUS_ICON_ROTATE_SP_MODE_LIB,
                                           dgus_pack_u8(0u, icon_lib));
}

dgus_status_t dgus_icon_rotate_set_centers(dgus_ui_t *ui,
                                           const dgus_icon_rotate_t *rot,
                                           dgus_point_t icon_center,
                                           dgus_point_t screen_center)
{
    uint16_t words[4];

    if (!dgus_icon_rotate_has_sp(rot)) {
        return DGUS_ERR_ARG;
    }

    words[0] = (uint16_t)icon_center.x;
    words[1] = (uint16_t)icon_center.y;
    words[2] = (uint16_t)screen_center.x;
    words[3] = (uint16_t)screen_center.y;

    return dgus_obj_desc_write_words_at(ui, &rot->obj, rot->sp,
                                 DGUS_ICON_ROTATE_SP_ICON_CENTER,
                                 words,
                                 4u);
}

dgus_status_t dgus_icon_rotate_set_value_range(dgus_ui_t *ui,
                                               const dgus_icon_rotate_t *rot,
                                               uint16_t begin_value,
                                               uint16_t end_value)
{
    uint16_t words[2];

    words[0] = begin_value;
    words[1] = end_value;

    if (!dgus_icon_rotate_has_sp(rot)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_words_at(ui, &rot->obj, rot->sp,
                                 DGUS_ICON_ROTATE_SP_V_BEGIN,
                                 words,
                                 2u);
}

dgus_status_t dgus_icon_rotate_set_angle_range(dgus_ui_t *ui,
                                               const dgus_icon_rotate_t *rot,
                                               uint16_t begin_angle_0_5deg,
                                               uint16_t end_angle_0_5deg)
{
    uint16_t words[2];

    words[0] = begin_angle_0_5deg;
    words[1] = end_angle_0_5deg;

    if (!dgus_icon_rotate_has_sp(rot)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_words_at(ui, &rot->obj, rot->sp,
                                 DGUS_ICON_ROTATE_SP_ANGLE_BEGIN,
                                 words,
                                 2u);
}
