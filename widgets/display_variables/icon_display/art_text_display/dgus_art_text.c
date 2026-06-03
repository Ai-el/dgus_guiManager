#include "dgus_art_text.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_art_text_t, dgus_art_text)
dgus_art_text_t *dgus_art_text_create(dgus_page_t *page,
                                      uint16_t id,
                                      uint16_t vp,
                                      uint16_t sp,
                                      uint8_t word_count)
{
    dgus_art_text_t *art;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    art = (dgus_art_text_t *)dgus_page_alloc(page, sizeof(*art));
    if (art == NULL) {
        return NULL;
    }
    dgus_widget_init_obj(&art->obj, id, DGUS_OBJ_ART_TEXT, vp, word_count);
    art->sp = sp;
    if (dgus_page_add_obj(page, &art->obj) != DGUS_OK) {
        return NULL;
    }

    return art;
}

bool dgus_art_text_has_sp(const dgus_art_text_t *art)
{
    return (art != NULL) && dgus_widget_has_sp(art->sp);
}

dgus_status_t dgus_art_text_set_u16(dgus_ui_t *ui,
                                    const dgus_art_text_t *art,
                                    uint16_t value)
{
    if (art == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_set_u16(ui, &art->obj, value);
}

dgus_status_t dgus_art_text_desc_write_u16(dgus_ui_t *ui,
                                           const dgus_art_text_t *art,
                                           uint16_t offset,
                                           uint16_t value)
{
    if (!dgus_art_text_has_sp(art)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &art->obj, art->sp, offset, value);
}

dgus_status_t dgus_art_text_set_pos(dgus_ui_t *ui,
                                    const dgus_art_text_t *art,
                                    int16_t x,
                                    int16_t y)
{
    dgus_point_t point;

    if (!dgus_art_text_has_sp(art)) {
        return DGUS_ERR_ARG;
    }

    point.x = x;
    point.y = y;

    return dgus_obj_desc_write_point_at(ui, &art->obj, art->sp, DGUS_ART_TEXT_SP_POS, point);
}

dgus_status_t dgus_art_text_set_icon0(dgus_ui_t *ui,
                                      const dgus_art_text_t *art,
                                      uint16_t icon0)
{
    return dgus_art_text_desc_write_u16(ui,
                                        art,
                                        DGUS_ART_TEXT_SP_ICON0,
                                        icon0);
}

dgus_status_t dgus_art_text_set_digits(dgus_ui_t *ui,
                                       const dgus_art_text_t *art,
                                       uint8_t integer_digits,
                                       uint8_t decimal_digits)
{
    return dgus_art_text_desc_write_u16(ui,
                                        art,
                                        DGUS_ART_TEXT_SP_DIGITS,
                                        dgus_pack_u8(integer_digits,
                                                     decimal_digits));
}
