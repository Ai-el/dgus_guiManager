#include "dgus_progress.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_progress_t, dgus_progress)
DGUS_WIDGET_DEFINE_CALLBACKS(dgus_bar_t, dgus_bar)
dgus_progress_t *dgus_progress_create(dgus_page_t *page,
                                      uint16_t id,
                                      uint16_t vp,
                                      uint16_t sp)
{
    dgus_progress_t *bar;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    bar = (dgus_progress_t *)dgus_page_alloc(page, sizeof(dgus_progress_t));
    if (bar == NULL) {
        return NULL;
    }
    dgus_widget_init_obj(&bar->obj, id, DGUS_OBJ_PROGRESS, vp, 1u);
    bar->sp = sp;
    bar->min_value = 0;
    bar->max_value = 100;
    if (dgus_page_add_obj(page, &bar->obj) != DGUS_OK) {
        return NULL;
    }

    return bar;
}

dgus_bar_t *dgus_bar_create(dgus_page_t *page,
                            uint16_t id,
                            uint16_t vp,
                            uint16_t sp)
{
    return dgus_progress_create(page, id, vp, sp);
}

bool dgus_progress_has_sp(const dgus_progress_t *bar)
{
    return (bar != NULL) && dgus_widget_has_sp(bar->sp);
}

void dgus_progress_set_range(dgus_progress_t *bar,
                             int32_t min_value,
                             int32_t max_value)
{
    if (bar != NULL) {
        bar->min_value = min_value;
        bar->max_value = max_value;
    }
}

void dgus_bar_set_range(dgus_bar_t *bar,
                        int32_t min_value,
                        int32_t max_value)
{
    dgus_progress_set_range(bar, min_value, max_value);
}

dgus_status_t dgus_progress_set_value(dgus_ui_t *ui,
                                      const dgus_progress_t *bar,
                                      uint16_t value)
{
    if (bar == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_set_u16(ui, &bar->obj, value);
}

dgus_status_t dgus_bar_set_value(dgus_ui_t *ui,
                                 const dgus_bar_t *bar,
                                 uint16_t value)
{
    return dgus_progress_set_value(ui, bar, value);
}

dgus_status_t dgus_progress_desc_write_u16(dgus_ui_t *ui,
                                           const dgus_progress_t *bar,
                                           uint16_t offset,
                                           uint16_t value)
{
    if (!dgus_progress_has_sp(bar)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &bar->obj, bar->sp, offset, value);
}

dgus_status_t dgus_progress_desc_write_words(dgus_ui_t *ui,
                                             const dgus_progress_t *bar,
                                             uint16_t offset,
                                             const uint16_t *words,
                                             uint8_t word_count)
{
    if (!dgus_progress_has_sp(bar)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_words_at(ui, &bar->obj, bar->sp, offset, words, word_count);
}

dgus_status_t dgus_progress_set_area(dgus_ui_t *ui,
                                     const dgus_progress_t *bar,
                                     dgus_rect_t area)
{
    if (!dgus_progress_has_sp(bar)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_rect_at(ui, &bar->obj, bar->sp, DGUS_PROGRESS_SP_START, area);
}

dgus_status_t dgus_progress_set_colors(dgus_ui_t *ui,
                                       const dgus_progress_t *bar,
                                       uint16_t border,
                                       uint16_t foreground,
                                       uint16_t background)
{
    uint16_t colors[3];

    colors[0] = border;
    colors[1] = foreground;
    colors[2] = background;

    return dgus_progress_desc_write_words(ui,
                                          bar,
                                          DGUS_PROGRESS_SP_BORDER,
                                          colors,
                                          3u);
}

dgus_status_t dgus_progress_set_value_limits(dgus_ui_t *ui,
                                             const dgus_progress_t *bar,
                                             int16_t min_value,
                                             int16_t max_value)
{
    uint16_t words[2];

    words[0] = (uint16_t)max_value;
    words[1] = (uint16_t)min_value;

    return dgus_progress_desc_write_words(ui,
                                          bar,
                                          DGUS_PROGRESS_SP_MAX,
                                          words,
                                          2u);
}

dgus_status_t dgus_progress_set_mode(dgus_ui_t *ui,
                                     const dgus_progress_t *bar,
                                     dgus_progress_frame_mode_t frame,
                                     dgus_progress_dir_t dir,
                                     bool return_percent)
{
    uint8_t mode = (uint8_t)frame & 0x03u;

    if (return_percent) {
        mode |= 0x80u;
    }

    return dgus_progress_desc_write_u16(ui,
                                        bar,
                                        DGUS_PROGRESS_SP_MODE_DIR,
                                        dgus_pack_u8(mode, (uint8_t)dir));
}

dgus_status_t dgus_progress_set_return_vp(dgus_ui_t *ui,
                                          const dgus_progress_t *bar,
                                          uint16_t return_vp)
{
    return dgus_progress_desc_write_u16(ui,
                                        bar,
                                        DGUS_PROGRESS_SP_RETURN_VP,
                                        return_vp);
}
