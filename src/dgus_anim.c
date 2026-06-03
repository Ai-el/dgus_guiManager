#include "dgus_anim.h"

#include <limits.h>
#include <stddef.h>
#include <string.h>

static int32_t dgus_anim_clamp_progress(int32_t progress)
{
    if (progress < 0) {
        return 0;
    }
    if (progress > DGUS_ANIM_PROGRESS_MAX) {
        return DGUS_ANIM_PROGRESS_MAX;
    }

    return progress;
}

static void dgus_anim_apply(dgus_anim_t *anim)
{
    int32_t progress;
    int32_t delta;
    int32_t value;

    if ((anim == NULL) || (anim->exec_cb == NULL)) {
        return;
    }

    if (anim->path_cb != NULL) {
        progress = anim->path_cb(anim);
    } else {
        progress = dgus_anim_path_linear(anim);
    }
    progress = dgus_anim_clamp_progress(progress);

    delta = anim->end_value - anim->start_value;
    value = anim->start_value +
            ((delta * progress) / DGUS_ANIM_PROGRESS_MAX);

    if (value != anim->current_value) {
        anim->current_value = value;
        anim->exec_cb(anim->var, value, anim->user_data);
    }
}

static void dgus_anim_finish_cycle(dgus_anim_t *anim)
{
    bool has_more;

    if (anim == NULL) {
        return;
    }

    has_more = (anim->repeat_left == DGUS_ANIM_REPEAT_INFINITE) ||
               (anim->repeat_left > 1u);

    if (has_more) {
        if (anim->repeat_left != DGUS_ANIM_REPEAT_INFINITE) {
            anim->repeat_left--;
        }
        anim->elapsed_ms = 0u;
        anim->delay_ms = 0u;
        if (anim->playback) {
            int32_t tmp = anim->start_value;
            anim->start_value = anim->end_value;
            anim->end_value = tmp;
            anim->reverse = !anim->reverse;
        }
        dgus_anim_apply(anim);
    } else {
        anim->active = false;
        if (anim->ready_cb != NULL) {
            anim->ready_cb(anim, anim->user_data);
        }
    }
}

void dgus_anim_manager_init(dgus_anim_manager_t *manager,
                            dgus_anim_t *items,
                            uint16_t capacity)
{
    if (manager == NULL) {
        return;
    }

    manager->items = items;
    manager->capacity = capacity;
    if (items != NULL) {
        memset(items, 0, sizeof(items[0]) * capacity);
    }
}

void dgus_anim_init(dgus_anim_t *anim)
{
    if (anim == NULL) {
        return;
    }

    memset(anim, 0, sizeof(*anim));
    anim->duration_ms = 300u;
    anim->repeat_count = 1u;
    anim->repeat_left = 1u;
    anim->path_cb = dgus_anim_path_linear;
    anim->current_value = INT32_MIN;
}

void dgus_anim_set_var(dgus_anim_t *anim, void *var)
{
    if (anim != NULL) {
        anim->var = var;
    }
}

void dgus_anim_set_values(dgus_anim_t *anim,
                          int32_t start_value,
                          int32_t end_value)
{
    if (anim != NULL) {
        anim->start_value = start_value;
        anim->end_value = end_value;
        anim->current_value = INT32_MIN;
    }
}

void dgus_anim_set_time(dgus_anim_t *anim, uint32_t duration_ms)
{
    if (anim != NULL) {
        anim->duration_ms = (duration_ms == 0u) ? 1u : duration_ms;
    }
}

void dgus_anim_set_delay(dgus_anim_t *anim, uint32_t delay_ms)
{
    if (anim != NULL) {
        anim->delay_ms = delay_ms;
    }
}

void dgus_anim_set_playback(dgus_anim_t *anim, bool enabled)
{
    if (anim != NULL) {
        anim->playback = enabled;
    }
}

void dgus_anim_set_repeat_count(dgus_anim_t *anim, uint16_t repeat_count)
{
    if (anim != NULL) {
        anim->repeat_count = (repeat_count == 0u) ? 1u : repeat_count;
        anim->repeat_left = anim->repeat_count;
    }
}

void dgus_anim_set_exec_cb(dgus_anim_t *anim, dgus_anim_exec_cb_t cb)
{
    if (anim != NULL) {
        anim->exec_cb = cb;
    }
}

void dgus_anim_set_ready_cb(dgus_anim_t *anim, dgus_anim_ready_cb_t cb)
{
    if (anim != NULL) {
        anim->ready_cb = cb;
    }
}

void dgus_anim_set_path_cb(dgus_anim_t *anim, dgus_anim_path_cb_t cb)
{
    if (anim != NULL) {
        anim->path_cb = (cb != NULL) ? cb : dgus_anim_path_linear;
    }
}

dgus_status_t dgus_anim_start(dgus_anim_manager_t *manager,
                              const dgus_anim_t *template_anim)
{
    uint16_t i;

    if ((manager == NULL) || (manager->items == NULL) ||
        (template_anim == NULL) || (template_anim->exec_cb == NULL) ||
        (template_anim->duration_ms == 0u)) {
        return DGUS_ERR_ARG;
    }

    for (i = 0u; i < manager->capacity; i++) {
        if (!manager->items[i].active) {
            manager->items[i] = *template_anim;
            manager->items[i].active = true;
            manager->items[i].reverse = false;
            manager->items[i].elapsed_ms = 0u;
            manager->items[i].repeat_left =
                (template_anim->repeat_count == 0u) ? 1u :
                template_anim->repeat_count;
            manager->items[i].current_value = INT32_MIN;
            if (manager->items[i].delay_ms == 0u) {
                dgus_anim_apply(&manager->items[i]);
            }
            return DGUS_OK;
        }
    }

    return DGUS_ERR_OVERFLOW;
}

void dgus_anim_del(dgus_anim_manager_t *manager,
                   void *var,
                   dgus_anim_exec_cb_t exec_cb)
{
    uint16_t i;

    if ((manager == NULL) || (manager->items == NULL)) {
        return;
    }

    for (i = 0u; i < manager->capacity; i++) {
        dgus_anim_t *anim = &manager->items[i];
        bool var_match = (var == NULL) || (anim->var == var);
        bool exec_match = (exec_cb == NULL) || (anim->exec_cb == exec_cb);

        if (anim->active && var_match && exec_match) {
            anim->active = false;
        }
    }
}

void dgus_anim_tick(dgus_anim_manager_t *manager, uint32_t elapsed_ms)
{
    uint16_t i;

    if ((manager == NULL) || (manager->items == NULL)) {
        return;
    }

    for (i = 0u; i < manager->capacity; i++) {
        dgus_anim_t *anim = &manager->items[i];
        uint32_t delta = elapsed_ms;

        if (!anim->active) {
            continue;
        }

        if (anim->delay_ms > 0u) {
            if (delta < anim->delay_ms) {
                anim->delay_ms -= delta;
                continue;
            }
            delta -= anim->delay_ms;
            anim->delay_ms = 0u;
            dgus_anim_apply(anim);
        }

        if (delta == 0u) {
            continue;
        }

        if ((UINT32_MAX - anim->elapsed_ms) < delta) {
            anim->elapsed_ms = anim->duration_ms;
        } else {
            anim->elapsed_ms += delta;
        }

        if (anim->elapsed_ms >= anim->duration_ms) {
            anim->elapsed_ms = anim->duration_ms;
            dgus_anim_apply(anim);
            dgus_anim_finish_cycle(anim);
        } else {
            dgus_anim_apply(anim);
        }
    }
}

bool dgus_anim_has_active(const dgus_anim_manager_t *manager)
{
    uint16_t i;

    if ((manager == NULL) || (manager->items == NULL)) {
        return false;
    }

    for (i = 0u; i < manager->capacity; i++) {
        if (manager->items[i].active) {
            return true;
        }
    }

    return false;
}

int32_t dgus_anim_get_progress(const dgus_anim_t *anim)
{
    uint64_t progress;

    if ((anim == NULL) || (anim->duration_ms == 0u)) {
        return DGUS_ANIM_PROGRESS_MAX;
    }

    if (anim->elapsed_ms >= anim->duration_ms) {
        return DGUS_ANIM_PROGRESS_MAX;
    }

    progress = ((uint64_t)anim->elapsed_ms * DGUS_ANIM_PROGRESS_MAX) /
               (uint64_t)anim->duration_ms;

    return (int32_t)progress;
}

int32_t dgus_anim_path_linear(const dgus_anim_t *anim)
{
    return dgus_anim_get_progress(anim);
}

int32_t dgus_anim_path_ease_in(const dgus_anim_t *anim)
{
    int32_t p = dgus_anim_get_progress(anim);

    return (p * p) / DGUS_ANIM_PROGRESS_MAX;
}

int32_t dgus_anim_path_ease_out(const dgus_anim_t *anim)
{
    int32_t p = DGUS_ANIM_PROGRESS_MAX - dgus_anim_get_progress(anim);

    return DGUS_ANIM_PROGRESS_MAX -
           ((p * p) / DGUS_ANIM_PROGRESS_MAX);
}

int32_t dgus_anim_path_ease_in_out(const dgus_anim_t *anim)
{
    int32_t p = dgus_anim_get_progress(anim);

    if (p < (DGUS_ANIM_PROGRESS_MAX / 2)) {
        return (2 * p * p) / DGUS_ANIM_PROGRESS_MAX;
    } else {
        int32_t q = DGUS_ANIM_PROGRESS_MAX - p;
        return DGUS_ANIM_PROGRESS_MAX -
               ((2 * q * q) / DGUS_ANIM_PROGRESS_MAX);
    }
}

static int16_t dgus_anim_lerp_i16(int16_t start,
                                  int16_t end,
                                  int32_t progress)
{
    int32_t delta = (int32_t)end - (int32_t)start;
    int32_t value = (int32_t)start +
                    ((delta * progress) / DGUS_ANIM_PROGRESS_MAX);

    if (value > INT16_MAX) {
        return INT16_MAX;
    }
    if (value < INT16_MIN) {
        return INT16_MIN;
    }

    return (int16_t)value;
}

static void dgus_anim_move_exec(void *var,
                                int32_t value,
                                void *user_data)
{
    dgus_anim_move_t *move = (dgus_anim_move_t *)var;
    int32_t progress = dgus_anim_clamp_progress(value);
    int16_t x;
    int16_t y;

    (void)user_data;

    if ((move == NULL) || (move->ui == NULL) || (move->target == NULL) ||
        (move->set_pos_cb == NULL)) {
        return;
    }

    x = dgus_anim_lerp_i16(move->start_x, move->end_x, progress);
    y = dgus_anim_lerp_i16(move->start_y, move->end_y, progress);

    if ((x == move->last_x) && (y == move->last_y)) {
        return;
    }

    if (move->set_pos_cb(move->ui, move->target, x, y) == DGUS_OK) {
        move->last_x = x;
        move->last_y = y;
    }
}

void dgus_anim_move_init(dgus_anim_move_t *move,
                         dgus_anim_t *anim,
                         dgus_ui_t *ui,
                         void *target,
                         dgus_anim_move_set_pos_cb_t set_pos_cb,
                         int16_t start_x,
                         int16_t start_y,
                         int16_t end_x,
                         int16_t end_y,
                         uint32_t duration_ms,
                         dgus_anim_path_cb_t path_cb)
{
    if ((move == NULL) || (anim == NULL)) {
        return;
    }

    move->ui = ui;
    move->target = target;
    move->set_pos_cb = set_pos_cb;
    move->start_x = start_x;
    move->start_y = start_y;
    move->end_x = end_x;
    move->end_y = end_y;
    move->last_x = INT16_MIN;
    move->last_y = INT16_MIN;

    dgus_anim_init(anim);
    dgus_anim_set_var(anim, move);
    dgus_anim_set_values(anim, 0, DGUS_ANIM_PROGRESS_MAX);
    dgus_anim_set_time(anim, duration_ms);
    dgus_anim_set_exec_cb(anim, dgus_anim_move_exec);
    dgus_anim_set_path_cb(anim, path_cb);
}

static dgus_status_t dgus_anim_data_var_set_pos(dgus_ui_t *ui,
                                                void *target,
                                                int16_t x,
                                                int16_t y)
{
    return dgus_data_var_set_pos(ui, (const dgus_data_var_t *)target, x, y);
}

static dgus_status_t dgus_anim_text_set_pos(dgus_ui_t *ui,
                                            void *target,
                                            int16_t x,
                                            int16_t y)
{
    return dgus_text_set_pos(ui, (const dgus_text_t *)target, x, y);
}

static dgus_status_t dgus_anim_var_icon_set_pos(dgus_ui_t *ui,
                                                void *target,
                                                int16_t x,
                                                int16_t y)
{
    return dgus_var_icon_set_pos(ui, (const dgus_var_icon_t *)target, x, y);
}

void dgus_anim_data_var_move_init(dgus_anim_move_t *move,
                                  dgus_anim_t *anim,
                                  dgus_ui_t *ui,
                                  dgus_data_var_t *data,
                                  int16_t start_x,
                                  int16_t start_y,
                                  int16_t end_x,
                                  int16_t end_y,
                                  uint32_t duration_ms,
                                  dgus_anim_path_cb_t path_cb)
{
    dgus_anim_move_init(move,
                        anim,
                        ui,
                        data,
                        dgus_anim_data_var_set_pos,
                        start_x,
                        start_y,
                        end_x,
                        end_y,
                        duration_ms,
                        path_cb);
}

void dgus_anim_text_move_init(dgus_anim_move_t *move,
                              dgus_anim_t *anim,
                              dgus_ui_t *ui,
                              dgus_text_t *text,
                              int16_t start_x,
                              int16_t start_y,
                              int16_t end_x,
                              int16_t end_y,
                              uint32_t duration_ms,
                              dgus_anim_path_cb_t path_cb)
{
    dgus_anim_move_init(move,
                        anim,
                        ui,
                        text,
                        dgus_anim_text_set_pos,
                        start_x,
                        start_y,
                        end_x,
                        end_y,
                        duration_ms,
                        path_cb);
}

void dgus_anim_var_icon_move_init(dgus_anim_move_t *move,
                                  dgus_anim_t *anim,
                                  dgus_ui_t *ui,
                                  dgus_var_icon_t *icon,
                                  int16_t start_x,
                                  int16_t start_y,
                                  int16_t end_x,
                                  int16_t end_y,
                                  uint32_t duration_ms,
                                  dgus_anim_path_cb_t path_cb)
{
    dgus_anim_move_init(move,
                        anim,
                        ui,
                        icon,
                        dgus_anim_var_icon_set_pos,
                        start_x,
                        start_y,
                        end_x,
                        end_y,
                        duration_ms,
                        path_cb);
}
