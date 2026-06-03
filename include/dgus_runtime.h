#ifndef DGUS_RUNTIME_H
#define DGUS_RUNTIME_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "dgus_anim.h"
#include "dgus_ui.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DGUS_RUNTIME_RX_CHUNK_SIZE
#define DGUS_RUNTIME_RX_CHUNK_SIZE 64u
#endif

#ifndef DGUS_RUNTIME_RX_READ_LIMIT
#define DGUS_RUNTIME_RX_READ_LIMIT 4u
#endif

#ifndef DGUS_RUNTIME_DEFAULT_HANDLER_PERIOD_MS
#define DGUS_RUNTIME_DEFAULT_HANDLER_PERIOD_MS 5u
#endif

#define DGUS_RUNTIME_POLL_DISABLED 0u

/* Optional platform tick getter. Return a monotonically increasing ms tick. */
typedef uint32_t (*dgus_tick_get_cb_t)(void *user_data);

/* Optional UART reader used by dgus_runtime_handler() to pump RX bytes. */
typedef size_t (*dgus_rx_read_cb_t)(uint8_t *data,
                                    size_t max_len,
                                    void *user_data);

/* One periodic VP read request managed by the runtime handler. */
typedef struct {
    bool enabled;
    uint16_t vp;
    uint8_t word_count;
    uint32_t period_ms;
    uint32_t next_run_ms;
} dgus_runtime_watch_t;

/* LVGL-like DGUS runtime context for one physical screen. */
typedef struct {
    dgus_t *dev;
    dgus_ui_t *ui;
    dgus_anim_manager_t *anim;
    dgus_rx_read_cb_t rx_read_cb;
    void *rx_read_user_data;
    uint8_t *rx_buffer;
    uint16_t rx_buffer_size;
    volatile uint16_t rx_head;
    volatile uint16_t rx_tail;
    volatile uint16_t rx_overflow_count;
    dgus_runtime_watch_t *watches;
    uint16_t watch_count;
    uint16_t watch_capacity;
    uint16_t next_watch_index;
    uint32_t last_handler_ms;
    uint32_t page_poll_period_ms;
    uint32_t page_poll_next_ms;
    uint32_t touch_poll_period_ms;
    uint32_t touch_poll_next_ms;
    uint8_t max_poll_tx_per_handler;
} dgus_runtime_t;

/* Increment the built-in millisecond tick, usually from SysTick. */
void dgus_tick_inc(uint32_t ms);

/* Return the current DGUS millisecond tick. */
uint32_t dgus_tick_get(void);

/* Return elapsed milliseconds since prev_ms using wrap-safe arithmetic. */
uint32_t dgus_tick_elaps(uint32_t prev_ms);

/* Override the built-in tick with a platform tick getter. */
void dgus_tick_set_cb(dgus_tick_get_cb_t cb, void *user_data);

/* Initialize a DGUS runtime context and connect protocol frames to the UI. */
void dgus_runtime_init(dgus_runtime_t *runtime,
                       dgus_t *dev,
                       dgus_ui_t *ui,
                       dgus_anim_manager_t *anim);

/* Set the optional UART RX reader pumped by dgus_runtime_handler(). */
void dgus_runtime_set_rx_reader(dgus_runtime_t *runtime,
                                dgus_rx_read_cb_t cb,
                                void *user_data);

/* Attach a user-owned RX ring buffer.
 * buffer: static byte array owned by the application.
 * size: buffer capacity in bytes. One byte is kept empty to distinguish full
 *   from empty, so useful capacity is size - 1.
 * The ring buffer lets a UART idle/DMA callback call dgus_runtime_rx_write()
 * quickly while dgus_timer_handler() performs parsing and event callbacks in
 * the main loop.
 */
void dgus_runtime_set_rx_buffer(dgus_runtime_t *runtime,
                                uint8_t *buffer,
                                uint16_t size);

/* Push received UART bytes into the runtime RX ring buffer.
 * Safe to call from a UART idle/DMA callback if uint16_t reads/writes are
 * atomic on the target. Returns DGUS_ERR_OVERFLOW if any byte was dropped.
 */
dgus_status_t dgus_runtime_rx_write(dgus_runtime_t *runtime,
                                    const uint8_t *data,
                                    uint16_t len);

/* Read bytes from the runtime RX ring buffer. Usually called by the handler. */
size_t dgus_runtime_rx_read(dgus_runtime_t *runtime,
                            uint8_t *data,
                            size_t max_len);

/* Return the number of bytes currently waiting in the runtime RX ring buffer. */
uint16_t dgus_runtime_rx_pending(const dgus_runtime_t *runtime);

/* Return the number of bytes dropped because the runtime RX buffer was full. */
uint16_t dgus_runtime_rx_overflow_count(const dgus_runtime_t *runtime);

/* Set a static VP watch table used for periodic reads. */
void dgus_runtime_set_watch_table(dgus_runtime_t *runtime,
                                  dgus_runtime_watch_t *watches,
                                  uint16_t capacity);

/* Add a periodic VP read request, similar to an LVGL timer. */
dgus_status_t dgus_runtime_watch_add(dgus_runtime_t *runtime,
                                     uint16_t vp,
                                     uint8_t word_count,
                                     uint32_t period_ms);

/* Enable or disable an existing VP watch by address. */
dgus_status_t dgus_runtime_watch_set_enabled(dgus_runtime_t *runtime,
                                             uint16_t vp,
                                             bool enabled);

/* Configure current-page polling from system variable 0x0014. */
void dgus_runtime_set_page_poll(dgus_runtime_t *runtime, uint32_t period_ms);

/* Configure touch-state polling from system variable 0x0016. */
void dgus_runtime_set_touch_poll(dgus_runtime_t *runtime, uint32_t period_ms);

/* Limit periodic read commands emitted by each handler call. */
void dgus_runtime_set_poll_tx_budget(dgus_runtime_t *runtime,
                                     uint8_t max_tx_per_handler);

/* Request a current-page read immediately. */
dgus_status_t dgus_runtime_sync_page_now(dgus_runtime_t *runtime);

/* Pump RX, run animations, and issue due DGUS polls. Returns suggested delay. */
uint32_t dgus_runtime_handler(dgus_runtime_t *runtime);

/* LVGL-style alias for dgus_runtime_handler(). */
uint32_t dgus_timer_handler(dgus_runtime_t *runtime);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_RUNTIME_H */
