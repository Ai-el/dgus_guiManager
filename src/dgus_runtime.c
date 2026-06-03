#include "dgus_runtime.h"

#include <limits.h>
#include <string.h>

static volatile uint32_t g_dgus_tick_ms;
static dgus_tick_get_cb_t g_dgus_tick_cb;
static void *g_dgus_tick_user_data;

static bool dgus_time_due(uint32_t now_ms, uint32_t due_ms)
{
    return ((int32_t)(now_ms - due_ms) >= 0);
}

static uint32_t dgus_time_until(uint32_t now_ms, uint32_t due_ms)
{
    if (dgus_time_due(now_ms, due_ms)) {
        return 0u;
    }

    return due_ms - now_ms;
}

static uint32_t dgus_min_u32(uint32_t a, uint32_t b)
{
    return (a < b) ? a : b;
}

void dgus_tick_inc(uint32_t ms)
{
    g_dgus_tick_ms += ms;
}

uint32_t dgus_tick_get(void)
{
    if (g_dgus_tick_cb != NULL) {
        return g_dgus_tick_cb(g_dgus_tick_user_data);
    }

    return g_dgus_tick_ms;
}

uint32_t dgus_tick_elaps(uint32_t prev_ms)
{
    return dgus_tick_get() - prev_ms;
}

void dgus_tick_set_cb(dgus_tick_get_cb_t cb, void *user_data)
{
    g_dgus_tick_cb = cb;
    g_dgus_tick_user_data = user_data;
}

void dgus_runtime_init(dgus_runtime_t *runtime,
                       dgus_t *dev,
                       dgus_ui_t *ui,
                       dgus_anim_manager_t *anim)
{
    uint32_t now_ms;

    if (runtime == NULL) {
        return;
    }

    memset(runtime, 0, sizeof(*runtime));
    now_ms = dgus_tick_get();
    runtime->dev = dev;
    runtime->ui = ui;
    runtime->anim = anim;
    runtime->last_handler_ms = now_ms;
    runtime->page_poll_period_ms = DGUS_RUNTIME_POLL_DISABLED;
    runtime->page_poll_next_ms = now_ms;
    runtime->touch_poll_period_ms = DGUS_RUNTIME_POLL_DISABLED;
    runtime->touch_poll_next_ms = now_ms;
    runtime->max_poll_tx_per_handler = 1u;

    if ((dev != NULL) && (ui != NULL)) {
        dgus_set_frame_callback(dev, dgus_ui_frame_callback, ui);
    }
}

void dgus_runtime_set_rx_reader(dgus_runtime_t *runtime,
                                dgus_rx_read_cb_t cb,
                                void *user_data)
{
    if (runtime != NULL) {
        runtime->rx_read_cb = cb;
        runtime->rx_read_user_data = user_data;
    }
}

void dgus_runtime_set_rx_buffer(dgus_runtime_t *runtime,
                                uint8_t *buffer,
                                uint16_t size)
{
    if (runtime == NULL) {
        return;
    }

    runtime->rx_buffer = buffer;
    runtime->rx_buffer_size = ((buffer != NULL) && (size > 1u)) ? size : 0u;
    runtime->rx_head = 0u;
    runtime->rx_tail = 0u;
    runtime->rx_overflow_count = 0u;
}

static uint16_t dgus_runtime_rx_next_index(const dgus_runtime_t *runtime,
                                           uint16_t index)
{
    index++;
    if ((runtime != NULL) && (index >= runtime->rx_buffer_size)) {
        index = 0u;
    }

    return index;
}

dgus_status_t dgus_runtime_rx_write(dgus_runtime_t *runtime,
                                    const uint8_t *data,
                                    uint16_t len)
{
    uint16_t i;
    dgus_status_t status = DGUS_OK;

    if ((runtime == NULL) || (runtime->rx_buffer == NULL) ||
        (runtime->rx_buffer_size < 2u) || ((data == NULL) && (len > 0u))) {
        return DGUS_ERR_ARG;
    }

    for (i = 0u; i < len; i++) {
        uint16_t head = runtime->rx_head;
        uint16_t next = dgus_runtime_rx_next_index(runtime, head);

        if (next == runtime->rx_tail) {
            runtime->rx_overflow_count++;
            status = DGUS_ERR_OVERFLOW;
            continue;
        }

        runtime->rx_buffer[head] = data[i];
        runtime->rx_head = next;
    }

    return status;
}

size_t dgus_runtime_rx_read(dgus_runtime_t *runtime,
                            uint8_t *data,
                            size_t max_len)
{
    size_t count = 0u;

    if ((runtime == NULL) || (runtime->rx_buffer == NULL) ||
        (runtime->rx_buffer_size < 2u) || (data == NULL)) {
        return 0u;
    }

    while ((count < max_len) && (runtime->rx_tail != runtime->rx_head)) {
        uint16_t tail = runtime->rx_tail;
        data[count] = runtime->rx_buffer[tail];
        runtime->rx_tail = dgus_runtime_rx_next_index(runtime, tail);
        count++;
    }

    return count;
}

uint16_t dgus_runtime_rx_pending(const dgus_runtime_t *runtime)
{
    uint16_t head;
    uint16_t tail;

    if ((runtime == NULL) || (runtime->rx_buffer == NULL) ||
        (runtime->rx_buffer_size < 2u)) {
        return 0u;
    }

    head = runtime->rx_head;
    tail = runtime->rx_tail;
    if (head >= tail) {
        return (uint16_t)(head - tail);
    }

    return (uint16_t)(runtime->rx_buffer_size - tail + head);
}

uint16_t dgus_runtime_rx_overflow_count(const dgus_runtime_t *runtime)
{
    if (runtime == NULL) {
        return 0u;
    }

    return runtime->rx_overflow_count;
}

void dgus_runtime_set_watch_table(dgus_runtime_t *runtime,
                                  dgus_runtime_watch_t *watches,
                                  uint16_t capacity)
{
    if (runtime == NULL) {
        return;
    }

    runtime->watches = watches;
    runtime->watch_capacity = capacity;
    runtime->watch_count = 0u;
    runtime->next_watch_index = 0u;
    if (watches != NULL) {
        memset(watches, 0, sizeof(watches[0]) * capacity);
    }
}

dgus_status_t dgus_runtime_watch_add(dgus_runtime_t *runtime,
                                     uint16_t vp,
                                     uint8_t word_count,
                                     uint32_t period_ms)
{
    dgus_runtime_watch_t *watch;

    if ((runtime == NULL) || (runtime->watches == NULL) ||
        (word_count == 0u) || (period_ms == 0u)) {
        return DGUS_ERR_ARG;
    }
    if (runtime->watch_count >= runtime->watch_capacity) {
        return DGUS_ERR_OVERFLOW;
    }

    watch = &runtime->watches[runtime->watch_count];
    watch->enabled = true;
    watch->vp = vp;
    watch->word_count = word_count;
    watch->period_ms = period_ms;
    watch->next_run_ms = dgus_tick_get();
    runtime->watch_count++;

    return DGUS_OK;
}

dgus_status_t dgus_runtime_watch_set_enabled(dgus_runtime_t *runtime,
                                             uint16_t vp,
                                             bool enabled)
{
    uint16_t i;

    if ((runtime == NULL) || (runtime->watches == NULL)) {
        return DGUS_ERR_ARG;
    }

    for (i = 0u; i < runtime->watch_count; i++) {
        if (runtime->watches[i].vp == vp) {
            runtime->watches[i].enabled = enabled;
            runtime->watches[i].next_run_ms = dgus_tick_get();
            return DGUS_OK;
        }
    }

    return DGUS_ERR_ARG;
}

void dgus_runtime_set_page_poll(dgus_runtime_t *runtime, uint32_t period_ms)
{
    if (runtime != NULL) {
        runtime->page_poll_period_ms = period_ms;
        runtime->page_poll_next_ms = dgus_tick_get();
    }
}

void dgus_runtime_set_touch_poll(dgus_runtime_t *runtime, uint32_t period_ms)
{
    if (runtime != NULL) {
        runtime->touch_poll_period_ms = period_ms;
        runtime->touch_poll_next_ms = dgus_tick_get();
    }
}

void dgus_runtime_set_poll_tx_budget(dgus_runtime_t *runtime,
                                     uint8_t max_tx_per_handler)
{
    if (runtime != NULL) {
        runtime->max_poll_tx_per_handler =
            (max_tx_per_handler == 0u) ? 1u : max_tx_per_handler;
    }
}

dgus_status_t dgus_runtime_sync_page_now(dgus_runtime_t *runtime)
{
    if ((runtime == NULL) || (runtime->ui == NULL)) {
        return DGUS_ERR_ARG;
    }

    return dgus_ui_request_active_page(runtime->ui);
}

static void dgus_runtime_pump_rx(dgus_runtime_t *runtime)
{
    uint8_t buffer[DGUS_RUNTIME_RX_CHUNK_SIZE];
    uint8_t i;

    if ((runtime == NULL) || (runtime->dev == NULL)) {
        return;
    }

    if (runtime->rx_buffer != NULL) {
        for (i = 0u; i < (uint8_t)DGUS_RUNTIME_RX_READ_LIMIT; i++) {
            size_t len = dgus_runtime_rx_read(runtime, buffer, sizeof(buffer));
            if (len == 0u) {
                break;
            }
            (void)dgus_feed(runtime->dev, buffer, len);
            if (len < sizeof(buffer)) {
                break;
            }
        }
    }

    if (runtime->rx_read_cb == NULL) {
        return;
    }

    for (i = 0u; i < (uint8_t)DGUS_RUNTIME_RX_READ_LIMIT; i++) {
        size_t len = runtime->rx_read_cb(buffer,
                                         sizeof(buffer),
                                         runtime->rx_read_user_data);
        if (len == 0u) {
            break;
        }
        (void)dgus_feed(runtime->dev, buffer, len);
        if (len < sizeof(buffer)) {
            break;
        }
    }
}

static void dgus_runtime_poll_builtin(dgus_runtime_t *runtime,
                                      uint32_t now_ms,
                                      uint8_t *tx_left)
{
    if ((runtime == NULL) || (runtime->dev == NULL) || (tx_left == NULL)) {
        return;
    }

    if ((*tx_left > 0u) &&
        (runtime->page_poll_period_ms != DGUS_RUNTIME_POLL_DISABLED) &&
        dgus_time_due(now_ms, runtime->page_poll_next_ms)) {
        (void)dgus_request_current_page(runtime->dev);
        runtime->page_poll_next_ms = now_ms + runtime->page_poll_period_ms;
        (*tx_left)--;
    }

    if ((*tx_left > 0u) &&
        (runtime->touch_poll_period_ms != DGUS_RUNTIME_POLL_DISABLED) &&
        dgus_time_due(now_ms, runtime->touch_poll_next_ms)) {
        (void)dgus_read_words(runtime->dev, DGUS_SYS_ADDR_TOUCH_STATE, 3u);
        runtime->touch_poll_next_ms = now_ms + runtime->touch_poll_period_ms;
        (*tx_left)--;
    }
}

static void dgus_runtime_poll_watches(dgus_runtime_t *runtime,
                                      uint32_t now_ms,
                                      uint8_t *tx_left)
{
    uint16_t checked = 0u;

    if ((runtime == NULL) || (runtime->dev == NULL) ||
        (runtime->watches == NULL) || (tx_left == NULL) ||
        (runtime->watch_count == 0u)) {
        return;
    }

    while ((*tx_left > 0u) && (checked < runtime->watch_count)) {
        dgus_runtime_watch_t *watch;

        if (runtime->next_watch_index >= runtime->watch_count) {
            runtime->next_watch_index = 0u;
        }

        watch = &runtime->watches[runtime->next_watch_index];
        runtime->next_watch_index++;
        checked++;

        if (!watch->enabled || !dgus_time_due(now_ms, watch->next_run_ms)) {
            continue;
        }

        (void)dgus_read_words(runtime->dev, watch->vp, watch->word_count);
        watch->next_run_ms = now_ms + watch->period_ms;
        (*tx_left)--;
    }
}

static uint32_t dgus_runtime_next_wait(const dgus_runtime_t *runtime,
                                       uint32_t now_ms)
{
    uint16_t i;
    uint32_t wait_ms = UINT32_MAX;

    if (runtime == NULL) {
        return DGUS_RUNTIME_DEFAULT_HANDLER_PERIOD_MS;
    }

    if ((runtime->rx_read_cb != NULL) ||
        (dgus_runtime_rx_pending(runtime) > 0u) ||
        ((runtime->anim != NULL) && dgus_anim_has_active(runtime->anim))) {
        wait_ms = dgus_min_u32(wait_ms,
                               DGUS_RUNTIME_DEFAULT_HANDLER_PERIOD_MS);
    }

    if (runtime->page_poll_period_ms != DGUS_RUNTIME_POLL_DISABLED) {
        wait_ms = dgus_min_u32(wait_ms,
                               dgus_time_until(now_ms,
                                               runtime->page_poll_next_ms));
    }
    if (runtime->touch_poll_period_ms != DGUS_RUNTIME_POLL_DISABLED) {
        wait_ms = dgus_min_u32(wait_ms,
                               dgus_time_until(now_ms,
                                               runtime->touch_poll_next_ms));
    }
    for (i = 0u; i < runtime->watch_count; i++) {
        if (runtime->watches[i].enabled) {
            wait_ms = dgus_min_u32(wait_ms,
                                   dgus_time_until(now_ms,
                                                   runtime->watches[i].next_run_ms));
        }
    }

    if (wait_ms == UINT32_MAX) {
        return DGUS_RUNTIME_DEFAULT_HANDLER_PERIOD_MS;
    }

    return wait_ms;
}

uint32_t dgus_runtime_handler(dgus_runtime_t *runtime)
{
    uint32_t now_ms;
    uint32_t elapsed_ms;
    uint8_t tx_left;

    if (runtime == NULL) {
        return DGUS_RUNTIME_DEFAULT_HANDLER_PERIOD_MS;
    }

    now_ms = dgus_tick_get();
    elapsed_ms = now_ms - runtime->last_handler_ms;
    runtime->last_handler_ms = now_ms;

    dgus_runtime_pump_rx(runtime);

    if (runtime->anim != NULL) {
        dgus_anim_tick(runtime->anim, elapsed_ms);
    }

    tx_left = runtime->max_poll_tx_per_handler;
    dgus_runtime_poll_builtin(runtime, now_ms, &tx_left);
    dgus_runtime_poll_watches(runtime, now_ms, &tx_left);

    return dgus_runtime_next_wait(runtime, now_ms);
}

uint32_t dgus_timer_handler(dgus_runtime_t *runtime)
{
    return dgus_runtime_handler(runtime);
}
