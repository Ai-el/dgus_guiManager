#include "dgus_hex.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_hex_t, dgus_hex)
dgus_hex_t *dgus_hex_create(dgus_page_t *page,
                            uint16_t id,
                            uint16_t vp,
                            uint16_t sp,
                            uint8_t byte_count)
{
    dgus_hex_t *hex;
    uint8_t words;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    hex = (dgus_hex_t *)dgus_page_alloc(page, sizeof(*hex));
    if (hex == NULL) {
        return NULL;
    }
    words = (byte_count == 0u) ? 1u : (uint8_t)((byte_count + 1u) / 2u);
    dgus_widget_init_obj(&hex->obj, id, DGUS_OBJ_HEX, vp, words);
    hex->sp = sp;
    if (dgus_page_add_obj(page, &hex->obj) != DGUS_OK) {
        return NULL;
    }

    return hex;
}

bool dgus_hex_has_sp(const dgus_hex_t *hex)
{
    return (hex != NULL) && dgus_widget_has_sp(hex->sp);
}

dgus_status_t dgus_hex_write_bytes(dgus_ui_t *ui,
                                   const dgus_hex_t *hex,
                                   const uint8_t *data,
                                   uint16_t byte_len)
{
    if (hex == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_write_bytes(ui, &hex->obj, data, byte_len);
}

dgus_status_t dgus_hex_write_words(dgus_ui_t *ui,
                                   const dgus_hex_t *hex,
                                   const uint16_t *words,
                                   uint8_t word_count)
{
    if (hex == NULL) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_write_words(ui, &hex->obj, words, word_count);
}

dgus_status_t dgus_hex_desc_write_u16(dgus_ui_t *ui,
                                      const dgus_hex_t *hex,
                                      uint16_t offset,
                                      uint16_t value)
{
    if (!dgus_hex_has_sp(hex)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &hex->obj, hex->sp, offset, value);
}

dgus_status_t dgus_hex_desc_write_words(dgus_ui_t *ui,
                                        const dgus_hex_t *hex,
                                        uint16_t offset,
                                        const uint16_t *words,
                                        uint8_t word_count)
{
    if (!dgus_hex_has_sp(hex)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_words_at(ui, &hex->obj, hex->sp, offset, words, word_count);
}
