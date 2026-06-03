#include "dgus.h"

#include <string.h>

enum {
    DGUS_RX_WAIT_HEAD_H = 0,
    DGUS_RX_WAIT_HEAD_L,
    DGUS_RX_WAIT_LEN,
    DGUS_RX_READ_BODY
};

static void dgus_rx_reset(dgus_t *dev)
{
    dev->rx.state = DGUS_RX_WAIT_HEAD_H;
    dev->rx.len = 0u;
    dev->rx.index = 0u;
}

static void dgus_report_error(dgus_t *dev, dgus_status_t status)
{
    if ((dev != NULL) && (dev->on_error != NULL)) {
        dev->on_error(dev, status, dev->on_error_user_data);
    }
}

uint16_t dgus_crc16_modbus(const uint8_t *data, size_t len)
{
    uint16_t crc = 0xFFFFu;
    size_t i;

    if (data == NULL) {
        return crc;
    }

    for (i = 0u; i < len; i++) {
        uint8_t bit;
        crc ^= (uint16_t)data[i];
        for (bit = 0u; bit < 8u; bit++) {
            if ((crc & 0x0001u) != 0u) {
                crc = (uint16_t)((crc >> 1u) ^ 0xA001u);
            } else {
                crc >>= 1u;
            }
        }
    }

    return crc;
}

uint16_t dgus_get_u16_be(const uint8_t *data)
{
    return (uint16_t)(((uint16_t)data[0] << 8u) | (uint16_t)data[1]);
}

void dgus_put_u16_be(uint8_t *data, uint16_t value)
{
    data[0] = (uint8_t)(value >> 8u);
    data[1] = (uint8_t)(value & 0xFFu);
}

void dgus_init(dgus_t *dev, const dgus_port_t *port)
{
    if (dev == NULL) {
        return;
    }

    memset(dev, 0, sizeof(*dev));
    if (port != NULL) {
        dev->port = *port;
    }
    dgus_rx_reset(dev);
}

void dgus_set_crc_enabled(dgus_t *dev, bool enabled)
{
    if (dev != NULL) {
        dev->crc_enabled = enabled;
    }
}

void dgus_set_frame_callback(dgus_t *dev,
                             dgus_frame_cb_t cb,
                             void *user_data)
{
    if (dev != NULL) {
        dev->on_frame = cb;
        dev->on_frame_user_data = user_data;
    }
}

void dgus_set_error_callback(dgus_t *dev,
                             dgus_error_cb_t cb,
                             void *user_data)
{
    if (dev != NULL) {
        dev->on_error = cb;
        dev->on_error_user_data = user_data;
    }
}

dgus_status_t dgus_send_cmd(dgus_t *dev,
                            uint8_t cmd,
                            const uint8_t *payload,
                            uint16_t payload_len)
{
    uint8_t frame[DGUS_TX_BUFFER_SIZE];
    uint16_t body_len;
    uint16_t total_len;
    uint16_t offset;

    if ((dev == NULL) || (dev->port.write == NULL)) {
        return DGUS_ERR_ARG;
    }
    if ((payload_len > 0u) && (payload == NULL)) {
        return DGUS_ERR_ARG;
    }

    body_len = (uint16_t)(1u + payload_len + (dev->crc_enabled ? 2u : 0u));
    if (body_len > 0xFFu) {
        return DGUS_ERR_OVERFLOW;
    }

    total_len = (uint16_t)(3u + body_len);
    if (total_len > (uint16_t)DGUS_TX_BUFFER_SIZE) {
        return DGUS_ERR_OVERFLOW;
    }

    frame[0] = DGUS_FRAME_HEAD_H;
    frame[1] = DGUS_FRAME_HEAD_L;
    frame[2] = (uint8_t)body_len;
    frame[3] = cmd;
    if (payload_len > 0u) {
        memcpy(&frame[4], payload, payload_len);
    }

    offset = (uint16_t)(4u + payload_len);
    if (dev->crc_enabled) {
        uint16_t crc = dgus_crc16_modbus(&frame[3], (size_t)(1u + payload_len));
        frame[offset] = (uint8_t)(crc & 0xFFu);
        frame[offset + 1u] = (uint8_t)(crc >> 8u);
    }

    return dev->port.write(frame, total_len, dev->port.user_data);
}

dgus_status_t dgus_write_bytes(dgus_t *dev,
                               uint16_t vp,
                               const uint8_t *data,
                               uint16_t byte_len)
{
    uint8_t payload[DGUS_TX_BUFFER_SIZE];

    if ((data == NULL) || (byte_len == 0u)) {
        return DGUS_ERR_ARG;
    }
    if ((uint16_t)(byte_len + 2u) > (uint16_t)DGUS_TX_BUFFER_SIZE) {
        return DGUS_ERR_OVERFLOW;
    }

    dgus_put_u16_be(payload, vp);
    memcpy(&payload[2], data, byte_len);

    return dgus_send_cmd(dev,
                         DGUS_CMD_WRITE_VAR,
                         payload,
                         (uint16_t)(byte_len + 2u));
}

dgus_status_t dgus_write_words(dgus_t *dev,
                               uint16_t vp,
                               const uint16_t *words,
                               uint8_t word_count)
{
    uint8_t payload[DGUS_TX_BUFFER_SIZE];
    uint16_t payload_len;
    uint8_t i;

    if ((words == NULL) || (word_count == 0u)) {
        return DGUS_ERR_ARG;
    }

    payload_len = (uint16_t)(2u + ((uint16_t)word_count * 2u));
    if (payload_len > (uint16_t)DGUS_TX_BUFFER_SIZE) {
        return DGUS_ERR_OVERFLOW;
    }

    dgus_put_u16_be(payload, vp);
    for (i = 0u; i < word_count; i++) {
        dgus_put_u16_be(&payload[2u + ((uint16_t)i * 2u)], words[i]);
    }

    return dgus_send_cmd(dev, DGUS_CMD_WRITE_VAR, payload, payload_len);
}

dgus_status_t dgus_write_u16(dgus_t *dev, uint16_t vp, uint16_t value)
{
    return dgus_write_words(dev, vp, &value, 1u);
}

dgus_status_t dgus_write_u32(dgus_t *dev, uint16_t vp, uint32_t value)
{
    uint16_t words[2];

    words[0] = (uint16_t)(value >> 16u);
    words[1] = (uint16_t)(value & 0xFFFFu);

    return dgus_write_words(dev, vp, words, 2u);
}

dgus_status_t dgus_read_words(dgus_t *dev, uint16_t vp, uint8_t word_count)
{
    uint8_t payload[3];

    if ((word_count == 0u) || (word_count > (uint8_t)DGUS_MAX_READ_WORDS)) {
        return DGUS_ERR_ARG;
    }

    dgus_put_u16_be(payload, vp);
    payload[2] = word_count;

    return dgus_send_cmd(dev, DGUS_CMD_READ_VAR, payload, sizeof(payload));
}

dgus_status_t dgus_request_version(dgus_t *dev)
{
    return dgus_read_words(dev, DGUS_SYS_ADDR_VERSION, 1u);
}

dgus_status_t dgus_request_current_page(dgus_t *dev)
{
    return dgus_read_words(dev, DGUS_SYS_ADDR_CURRENT_PAGE, 1u);
}

dgus_status_t dgus_set_page(dgus_t *dev, uint16_t page_id)
{
    uint16_t words[2];

    words[0] = 0x5A01u;
    words[1] = page_id;

    return dgus_write_words(dev, DGUS_SYS_ADDR_PIC_SET, words, 2u);
}

dgus_status_t dgus_set_backlight(dgus_t *dev, uint8_t percent)
{
    if (percent > 100u) {
        return DGUS_ERR_ARG;
    }

    return dgus_write_bytes(dev, DGUS_SYS_ADDR_BACKLIGHT, &percent, 1u);
}

dgus_status_t dgus_set_backlight_standby(dgus_t *dev,
                                         uint8_t active_percent,
                                         uint8_t standby_percent,
                                         uint16_t timeout_10ms)
{
    uint8_t data[4];

    if ((active_percent > 100u) || (standby_percent > 100u)) {
        return DGUS_ERR_ARG;
    }

    data[0] = active_percent;
    data[1] = standby_percent;
    dgus_put_u16_be(&data[2], timeout_10ms);

    return dgus_write_bytes(dev, DGUS_SYS_ADDR_BACKLIGHT, data, sizeof(data));
}

dgus_status_t dgus_beep_8ms(dgus_t *dev, uint16_t duration_8ms)
{
    return dgus_write_u16(dev, DGUS_SYS_ADDR_BEEP, duration_8ms);
}

dgus_status_t dgus_beep_ms(dgus_t *dev, uint16_t duration_ms)
{
    uint16_t ticks = (uint16_t)((duration_ms + 7u) / 8u);

    if (ticks == 0u) {
        ticks = 1u;
    }

    return dgus_beep_8ms(dev, ticks);
}

dgus_status_t dgus_reset(dgus_t *dev)
{
    uint16_t words[2];

    words[0] = 0x55AAu;
    words[1] = 0x5AA5u;

    return dgus_write_words(dev, DGUS_SYS_ADDR_RESET, words, 2u);
}

dgus_status_t dgus_touch_simulate(dgus_t *dev,
                                  uint16_t mode,
                                  uint16_t x,
                                  uint16_t y)
{
    uint16_t words[4];

    words[0] = 0x5AA5u;
    words[1] = mode;
    words[2] = x;
    words[3] = y;

    return dgus_write_words(dev, DGUS_SYS_ADDR_TOUCH_SIM, words, 4u);
}

dgus_status_t dgus_touch_click(dgus_t *dev, uint16_t x, uint16_t y)
{
    return dgus_touch_simulate(dev, 0x0004u, x, y);
}

dgus_status_t dgus_touch_key(dgus_t *dev, uint8_t touch_code)
{
    return dgus_touch_simulate(dev,
                               0x0004u,
                               (uint16_t)(0xFF00u | touch_code),
                               0x0001u);
}

static dgus_status_t dgus_finish_rx_frame(dgus_t *dev)
{
    dgus_frame_t frame;
    uint16_t body_len;
    uint16_t payload_len;

    body_len = dev->rx.len;
    if (body_len < 1u) {
        dgus_report_error(dev, DGUS_ERR_FRAME);
        return DGUS_ERR_FRAME;
    }

    if (dev->crc_enabled) {
        uint16_t crc_calc;
        uint16_t crc_recv;

        if (body_len < 3u) {
            dgus_report_error(dev, DGUS_ERR_FRAME);
            return DGUS_ERR_FRAME;
        }

        crc_calc = dgus_crc16_modbus(dev->rx.body, (size_t)(body_len - 2u));
        crc_recv = (uint16_t)(((uint16_t)dev->rx.body[body_len - 1u] << 8u) |
                              (uint16_t)dev->rx.body[body_len - 2u]);
        if (crc_calc != crc_recv) {
            dgus_report_error(dev, DGUS_ERR_CRC);
            return DGUS_ERR_CRC;
        }
        payload_len = (uint16_t)(body_len - 3u);
    } else {
        payload_len = (uint16_t)(body_len - 1u);
    }

    frame.cmd = dev->rx.body[0];
    frame.payload = &dev->rx.body[1];
    frame.payload_len = payload_len;

    if (dev->on_frame != NULL) {
        dev->on_frame(dev, &frame, dev->on_frame_user_data);
    }

    return DGUS_OK;
}

dgus_status_t dgus_feed(dgus_t *dev, const uint8_t *data, size_t len)
{
    size_t i;
    dgus_status_t result = DGUS_OK;

    if ((dev == NULL) || ((data == NULL) && (len > 0u))) {
        return DGUS_ERR_ARG;
    }

    for (i = 0u; i < len; i++) {
        uint8_t byte = data[i];

        switch (dev->rx.state) {
        case DGUS_RX_WAIT_HEAD_H:
            if (byte == DGUS_FRAME_HEAD_H) {
                dev->rx.state = DGUS_RX_WAIT_HEAD_L;
            }
            break;

        case DGUS_RX_WAIT_HEAD_L:
            if (byte == DGUS_FRAME_HEAD_L) {
                dev->rx.state = DGUS_RX_WAIT_LEN;
            } else if (byte != DGUS_FRAME_HEAD_H) {
                dev->rx.state = DGUS_RX_WAIT_HEAD_H;
            }
            break;

        case DGUS_RX_WAIT_LEN:
            if ((byte == 0u)
#if DGUS_RX_BUFFER_SIZE < 255u
                || (byte > (uint8_t)DGUS_RX_BUFFER_SIZE)
#endif
            ) {
                dgus_rx_reset(dev);
                dgus_report_error(dev, DGUS_ERR_OVERFLOW);
                if (result == DGUS_OK) {
                    result = DGUS_ERR_OVERFLOW;
                }
            } else {
                dev->rx.len = byte;
                dev->rx.index = 0u;
                dev->rx.state = DGUS_RX_READ_BODY;
            }
            break;

        case DGUS_RX_READ_BODY:
            dev->rx.body[dev->rx.index] = byte;
            dev->rx.index++;
            if (dev->rx.index >= dev->rx.len) {
                dgus_status_t status = dgus_finish_rx_frame(dev);
                dgus_rx_reset(dev);
                if ((status != DGUS_OK) && (result == DGUS_OK)) {
                    result = status;
                }
            }
            break;

        default:
            dgus_rx_reset(dev);
            break;
        }
    }

    return result;
}

bool dgus_decode_var_frame(const dgus_frame_t *frame,
                           uint16_t *vp,
                           uint8_t *word_count,
                           const uint8_t **word_data)
{
    uint8_t words;
    uint16_t expected_len;

    if ((frame == NULL) || (frame->cmd != DGUS_CMD_READ_VAR) ||
        (frame->payload == NULL) || (frame->payload_len < 3u)) {
        return false;
    }

    words = frame->payload[2];
    expected_len = (uint16_t)(3u + ((uint16_t)words * 2u));
    if (frame->payload_len < expected_len) {
        return false;
    }

    if (vp != NULL) {
        *vp = dgus_get_u16_be(frame->payload);
    }
    if (word_count != NULL) {
        *word_count = words;
    }
    if (word_data != NULL) {
        *word_data = &frame->payload[3];
    }

    return true;
}

bool dgus_is_write_ok_frame(const dgus_frame_t *frame)
{
    return (frame != NULL) &&
           (frame->cmd == DGUS_CMD_WRITE_VAR) &&
           (frame->payload != NULL) &&
           (frame->payload_len >= 2u) &&
           (frame->payload[0] == DGUS_WRITE_ACK_H) &&
           (frame->payload[1] == DGUS_WRITE_ACK_L);
}
