#ifndef DGUS_H
#define DGUS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "dgus_conf.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DGUS_FRAME_HEAD_H              0x5Au
#define DGUS_FRAME_HEAD_L              0xA5u

#define DGUS_CMD_WRITE_VAR             0x82u
#define DGUS_CMD_READ_VAR              0x83u

#define DGUS_SYS_ADDR_RESET            0x0004u
#define DGUS_SYS_ADDR_VERSION          0x000Fu
#define DGUS_SYS_ADDR_RTC              0x0010u
#define DGUS_SYS_ADDR_CURRENT_PAGE     0x0014u
#define DGUS_SYS_ADDR_TOUCH_STATE      0x0016u
#define DGUS_SYS_ADDR_BACKLIGHT        0x0082u
#define DGUS_SYS_ADDR_PIC_SET          0x0084u
#define DGUS_SYS_ADDR_BEEP             0x00A0u
#define DGUS_SYS_ADDR_TOUCH_SIM        0x00D4u

#define DGUS_WRITE_ACK_H               0x4Fu
#define DGUS_WRITE_ACK_L               0x4Bu

typedef enum {
    DGUS_OK = 0,
    DGUS_ERR_ARG = -1,
    DGUS_ERR_IO = -2,
    DGUS_ERR_OVERFLOW = -3,
    DGUS_ERR_CRC = -4,
    DGUS_ERR_FRAME = -5
} dgus_status_t;

struct dgus;

/* Low-level byte writer used by the protocol driver to transmit one frame. */
typedef dgus_status_t (*dgus_write_cb_t)(const uint8_t *data,
                                         size_t len,
                                         void *user_data);

/* Port adapter that connects the DGUS driver to a board UART implementation. */
typedef struct {
    dgus_write_cb_t write;
    void *user_data;
} dgus_port_t;

/* Decoded DGUS frame body delivered by the RX parser. */
typedef struct {
    uint8_t cmd;
    const uint8_t *payload;
    uint16_t payload_len;
} dgus_frame_t;

/* Called after one valid DGUS frame is received and decoded. */
typedef void (*dgus_frame_cb_t)(struct dgus *dev,
                                const dgus_frame_t *frame,
                                void *user_data);

/* Called when the RX parser detects a frame, CRC, or buffer error. */
typedef void (*dgus_error_cb_t)(struct dgus *dev,
                                dgus_status_t status,
                                void *user_data);

/* Internal byte-stream parser state. Keep it inside dgus_t. */
typedef struct {
    uint8_t state;
    uint8_t len;
    uint8_t index;
    uint8_t body[DGUS_RX_BUFFER_SIZE];
} dgus_rx_parser_t;

/* Main DGUS driver instance. Keep one instance per physical display. */
typedef struct dgus {
    dgus_port_t port;
    bool crc_enabled;
    dgus_rx_parser_t rx;
    dgus_frame_cb_t on_frame;
    void *on_frame_user_data;
    dgus_error_cb_t on_error;
    void *on_error_user_data;
} dgus_t;

/* Initialize a DGUS driver instance with a board port adapter. */
void dgus_init(dgus_t *dev, const dgus_port_t *port);

/* Enable or disable protocol CRC. This must match the screen CFG setting. */
void dgus_set_crc_enabled(dgus_t *dev, bool enabled);

/* Register a callback that receives decoded DGUS frames. */
void dgus_set_frame_callback(dgus_t *dev,
                             dgus_frame_cb_t cb,
                             void *user_data);

/* Register a callback that receives RX parser errors. */
void dgus_set_error_callback(dgus_t *dev,
                             dgus_error_cb_t cb,
                             void *user_data);

/* Feed raw UART bytes into the DGUS RX state machine. */
dgus_status_t dgus_feed(dgus_t *dev, const uint8_t *data, size_t len);

/* Build and send one DGUS command frame. */
dgus_status_t dgus_send_cmd(dgus_t *dev,
                            uint8_t cmd,
                            const uint8_t *payload,
                            uint16_t payload_len);

/* Write raw bytes to a DGUS VP address. */
dgus_status_t dgus_write_bytes(dgus_t *dev,
                               uint16_t vp,
                               const uint8_t *data,
                               uint16_t byte_len);

/* Write big-endian 16-bit words to a DGUS VP address. */
dgus_status_t dgus_write_words(dgus_t *dev,
                               uint16_t vp,
                               const uint16_t *words,
                               uint8_t word_count);

/* Write one unsigned 16-bit word to a DGUS VP address. */
dgus_status_t dgus_write_u16(dgus_t *dev, uint16_t vp, uint16_t value);

/* Write one unsigned 32-bit value as two DGUS words. */
dgus_status_t dgus_write_u32(dgus_t *dev, uint16_t vp, uint32_t value);

/* Request word_count words from a DGUS VP address. */
dgus_status_t dgus_read_words(dgus_t *dev, uint16_t vp, uint8_t word_count);

/* Request the GUI/DWIN OS version word from 0x000F. */
dgus_status_t dgus_request_version(dgus_t *dev);

/* Request the current page ID from 0x0014. */
dgus_status_t dgus_request_current_page(dgus_t *dev);

/* Switch the screen background page through system variable 0x0084. */
dgus_status_t dgus_set_page(dgus_t *dev, uint16_t page_id);

/* Set full-screen backlight brightness in percent, 0..100. */
dgus_status_t dgus_set_backlight(dgus_t *dev, uint8_t percent);

/* Configure active/standby backlight and standby timeout in 10 ms units. */
dgus_status_t dgus_set_backlight_standby(dgus_t *dev,
                                         uint8_t active_percent,
                                         uint8_t standby_percent,
                                         uint16_t timeout_10ms);

/* Trigger the buzzer using DGUS 8 ms time units. */
dgus_status_t dgus_beep_8ms(dgus_t *dev, uint16_t duration_8ms);

/* Trigger the buzzer using milliseconds; the value is rounded up to 8 ms. */
dgus_status_t dgus_beep_ms(dgus_t *dev, uint16_t duration_ms);

/* Reset the T5L CPU by writing 0x55AA,0x5AA5 to 0x0004. */
dgus_status_t dgus_reset(dgus_t *dev);

/* Simulate a DGUS touch operation through system variable 0x00D4. */
dgus_status_t dgus_touch_simulate(dgus_t *dev,
                                  uint16_t mode,
                                  uint16_t x,
                                  uint16_t y);

/* Simulate a touch click at X,Y. */
dgus_status_t dgus_touch_click(dgus_t *dev, uint16_t x, uint16_t y);

/* Simulate a DGUS keyed touch action by touch-control code. */
dgus_status_t dgus_touch_key(dgus_t *dev, uint8_t touch_code);

/* Decode a 0x83 VP response or touch upload frame. */
bool dgus_decode_var_frame(const dgus_frame_t *frame,
                           uint16_t *vp,
                           uint8_t *word_count,
                           const uint8_t **word_data);

/* Return true when the frame is the normal 0x82 "OK" write response. */
bool dgus_is_write_ok_frame(const dgus_frame_t *frame);

/* Calculate the CRC16/MODBUS value used by DGUS when CRC is enabled. */
uint16_t dgus_crc16_modbus(const uint8_t *data, size_t len);

/* Read a big-endian 16-bit value from a byte buffer. */
uint16_t dgus_get_u16_be(const uint8_t *data);

/* Write a big-endian 16-bit value to a byte buffer. */
void dgus_put_u16_be(uint8_t *data, uint16_t value);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_H */
