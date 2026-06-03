#include "dgus_batch_icon.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_batch_icon_t, dgus_batch_icon)
dgus_batch_icon_t *dgus_batch_icon_create(dgus_page_t *page,
                                          uint16_t id,
                                          uint16_t vp,
                                          uint16_t sp,
                                          uint16_t item_count)
{
    dgus_batch_icon_t *icon;
    uint8_t words = 1u;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    icon = (dgus_batch_icon_t *)dgus_page_alloc(page, sizeof(*icon));
    if (icon == NULL) {
        return NULL;
    }
    if (item_count > 0u) {
        words = (item_count > 127u) ? 254u : (uint8_t)(item_count * 2u);
    }
    dgus_widget_init_obj(&icon->obj, id, DGUS_OBJ_BATCH_ICON, vp, words);
    icon->sp = sp;
    if (dgus_page_add_obj(page, &icon->obj) != DGUS_OK) {
        return NULL;
    }

    return icon;
}

bool dgus_batch_icon_has_sp(const dgus_batch_icon_t *icon)
{
    return (icon != NULL) && dgus_widget_has_sp(icon->sp);
}

dgus_status_t dgus_batch_icon_set_item(dgus_ui_t *ui,
                                       const dgus_batch_icon_t *icon,
                                       uint16_t item_index,
                                       dgus_batch_icon_item_t item)
{
    uint8_t data[4];

    if (icon == NULL) {
        return DGUS_ERR_ARG;
    }
    data[0] = item.valid_marker;
    data[1] = item.icon_id;
    data[2] = item.mode_filter;
    data[3] = item.brightness;

    return dgus_obj_write_bytes_at(ui,
                                   &icon->obj,
                                   (uint16_t)(item_index * 2u),
                                   data,
                                   sizeof(data));
}

dgus_status_t dgus_batch_icon_desc_write_u16(dgus_ui_t *ui,
                                             const dgus_batch_icon_t *icon,
                                             uint16_t offset,
                                             uint16_t value)
{
    if (!dgus_batch_icon_has_sp(icon)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &icon->obj, icon->sp, offset, value);
}

dgus_status_t dgus_batch_icon_set_sources(dgus_ui_t *ui,
                                          const dgus_batch_icon_t *icon,
                                          uint16_t data_vp,
                                          uint16_t position_vp,
                                          uint16_t copy_desc_vp)
{
    uint16_t words[3];

    words[0] = data_vp;
    words[1] = position_vp;
    words[2] = copy_desc_vp;
    if (!dgus_batch_icon_has_sp(icon)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_words_at(ui, &icon->obj, icon->sp,
                                 DGUS_BATCH_ICON_SP_DATA_VP,
                                 words,
                                 3u);
}

dgus_status_t dgus_batch_icon_set_count(dgus_ui_t *ui,
                                        const dgus_batch_icon_t *icon,
                                        uint16_t item_count)
{
    return dgus_batch_icon_desc_write_u16(ui,
                                          icon,
                                          DGUS_BATCH_ICON_SP_DATA_COUNT,
                                          item_count);
}

dgus_status_t dgus_batch_icon_set_background(dgus_ui_t *ui,
                                             const dgus_batch_icon_t *icon,
                                             uint8_t icon_source,
                                             uint8_t icon_lib,
                                             uint16_t icon_id)
{
    uint16_t words[2];

    words[0] = dgus_pack_u8(icon_source, icon_lib);
    words[1] = icon_id;
    if (!dgus_batch_icon_has_sp(icon)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_words_at(ui, &icon->obj, icon->sp,
                                 DGUS_BATCH_ICON_SP_SOURCE_LIB,
                                 words,
                                 2u);
}
