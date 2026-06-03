#include "dgus_qr.h"

#include "dgus_widget_common.h"


DGUS_WIDGET_DEFINE_CALLBACKS(dgus_qr_t, dgus_qr)
dgus_qr_t *dgus_qr_create(dgus_page_t *page,
                          uint16_t id,
                          uint16_t vp,
                          uint16_t sp,
                          uint16_t max_bytes)
{
    dgus_qr_t *qr;
    uint8_t words;

    if ((page == NULL) || (page->object_count >= page->object_capacity)) {
        return NULL;
    }
    qr = (dgus_qr_t *)dgus_page_alloc(page, sizeof(*qr));
    if (qr == NULL) {
        return NULL;
    }

    words = (max_bytes > 254u) ? 127u : (uint8_t)((max_bytes + 1u) / 2u);
    dgus_widget_init_obj(&qr->obj, id, DGUS_OBJ_QR, vp, words);
    qr->sp = sp;
    qr->max_bytes = max_bytes;
    if (dgus_page_add_obj(page, &qr->obj) != DGUS_OK) {
        return NULL;
    }

    return qr;
}

bool dgus_qr_has_sp(const dgus_qr_t *qr)
{
    return (qr != NULL) && dgus_widget_has_sp(qr->sp);
}

dgus_status_t dgus_qr_set_text_ascii(dgus_ui_t *ui,
                                     const dgus_qr_t *qr,
                                     const char *text)
{
    uint8_t data[DGUS_TX_BUFFER_SIZE];
    uint16_t len = 0u;

    if ((ui == NULL) || (ui->dev == NULL) || (qr == NULL) || (text == NULL)) {
        return DGUS_ERR_ARG;
    }

    while ((text[len] != '\0') && (len < qr->max_bytes)) {
        if (len >= (uint16_t)(sizeof(data) - 2u)) {
            return DGUS_ERR_OVERFLOW;
        }
        data[len] = (uint8_t)text[len];
        len++;
    }

    if ((uint16_t)(len + 2u) > (uint16_t)sizeof(data)) {
        return DGUS_ERR_OVERFLOW;
    }
    data[len] = 0xFFu;
    data[len + 1u] = 0xFFu;

    return dgus_obj_write_bytes(ui,
                                &qr->obj,
                                data,
                                (uint16_t)(len + 2u));
}

dgus_status_t dgus_qr_desc_write_u16(dgus_ui_t *ui,
                                     const dgus_qr_t *qr,
                                     uint16_t offset,
                                     uint16_t value)
{
    if (!dgus_qr_has_sp(qr)) {
        return DGUS_ERR_ARG;
    }

    return dgus_obj_desc_write_u16_at(ui, &qr->obj, qr->sp, offset, value);
}

dgus_status_t dgus_qr_set_pos(dgus_ui_t *ui,
                              const dgus_qr_t *qr,
                              int16_t x,
                              int16_t y)
{
    dgus_point_t point;

    if (!dgus_qr_has_sp(qr)) {
        return DGUS_ERR_ARG;
    }

    point.x = x;
    point.y = y;

    return dgus_obj_desc_write_point_at(ui, &qr->obj, qr->sp, DGUS_QR_SP_POS, point);
}

dgus_status_t dgus_qr_set_unit_pixels(dgus_ui_t *ui,
                                      const dgus_qr_t *qr,
                                      uint16_t unit_pixels)
{
    return dgus_qr_desc_write_u16(ui, qr, DGUS_QR_SP_UNIT_PIXELS, unit_pixels);
}

dgus_status_t dgus_qr_set_fixed_73(dgus_ui_t *ui,
                                   const dgus_qr_t *qr,
                                   bool fixed)
{
    return dgus_qr_desc_write_u16(ui,
                                  qr,
                                  DGUS_QR_SP_FIX_MODE,
                                  dgus_pack_u8(fixed ? 1u : 0u, 0u));
}
