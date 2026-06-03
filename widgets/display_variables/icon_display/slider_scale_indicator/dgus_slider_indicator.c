#include "dgus_slider_indicator.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_slider_indicator_t, dgus_slider_indicator)
dgus_slider_indicator_t *dgus_slider_indicator_create(dgus_page_t *page,
                                                      uint16_t id,
                                                      uint16_t vp,
                                                      uint16_t sp)
{
    dgus_slider_indicator_t *slider;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    slider = (dgus_slider_indicator_t *)dgus_page_alloc(page, sizeof(*slider));
    if (slider == NULL) {
        return NULL;
    }
    dgus_widget_init_obj(&slider->obj,
                         id,
                         DGUS_OBJ_SLIDER_INDICATOR,
                         vp,
                         1u);
    slider->sp = sp;
    if (dgus_page_add_obj(page, &slider->obj) != DGUS_OK) {
        return NULL;
    }

    return slider;
}

bool dgus_slider_indicator_has_sp(const dgus_slider_indicator_t *slider)
{
    return (slider != NULL) && dgus_widget_has_sp(slider->sp);
}

dgus_status_t dgus_slider_indicator_set_value(dgus_ui_t *ui,
                                              const dgus_slider_indicator_t *slider,
                                              uint16_t value)
{
    if (slider == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_set_u16(ui, &slider->obj, value);
}

dgus_status_t dgus_slider_indicator_desc_write_u16(dgus_ui_t *ui,
                                                   const dgus_slider_indicator_t *slider,
                                                   uint16_t offset,
                                                   uint16_t value)
{
    if (!dgus_slider_indicator_has_sp(slider)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &slider->obj, slider->sp, offset, value);
}

dgus_status_t dgus_slider_indicator_set_value_range(dgus_ui_t *ui,
                                                    const dgus_slider_indicator_t *slider,
                                                    uint16_t begin_value,
                                                    uint16_t end_value)
{
    uint16_t words[2];

    words[0] = begin_value;
    words[1] = end_value;

    if (!dgus_slider_indicator_has_sp(slider)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_words_at(ui, &slider->obj, slider->sp,
                                 DGUS_SLIDER_SP_V_BEGIN,
                                 words,
                                 2u);
}

dgus_status_t dgus_slider_indicator_set_axis_range(dgus_ui_t *ui,
                                                   const dgus_slider_indicator_t *slider,
                                                   uint16_t begin_axis,
                                                   uint16_t end_axis)
{
    uint16_t words[2];

    words[0] = begin_axis;
    words[1] = end_axis;

    if (!dgus_slider_indicator_has_sp(slider)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_words_at(ui, &slider->obj, slider->sp,
                                 DGUS_SLIDER_SP_X_BEGIN,
                                 words,
                                 2u);
}

dgus_status_t dgus_slider_indicator_set_icon(dgus_ui_t *ui,
                                             const dgus_slider_indicator_t *slider,
                                             uint16_t icon_id,
                                             uint16_t fixed_axis)
{
    uint16_t words[2];

    words[0] = icon_id;
    words[1] = fixed_axis;

    if (!dgus_slider_indicator_has_sp(slider)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_words_at(ui, &slider->obj, slider->sp,
                                 DGUS_SLIDER_SP_ICON_ID,
                                 words,
                                 2u);
}

dgus_status_t dgus_slider_indicator_set_direction(dgus_ui_t *ui,
                                                  const dgus_slider_indicator_t *slider,
                                                  dgus_slider_dir_t dir)
{
    return dgus_slider_indicator_desc_write_u16(ui,
                                                slider,
                                                DGUS_SLIDER_SP_ADJ_MODE,
                                                dgus_pack_u8(0u,
                                                             (uint8_t)dir));
}
