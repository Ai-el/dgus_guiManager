#ifndef DGUS_ANIM_H
#define DGUS_ANIM_H

#include <stdbool.h>
#include <stdint.h>

#include "../widgets/display_variables/icon_display/variable_icon_display/dgus_var_icon.h"
#include "../widgets/display_variables/text_variables/data_variable_display/dgus_data_var.h"
#include "../widgets/display_variables/text_variables/text_display/dgus_text.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DGUS_ANIM_PROGRESS_MAX      1024
#define DGUS_ANIM_REPEAT_INFINITE   0xFFFFu

struct dgus_anim;

/* Called by an animation when a new interpolated value is available. */
typedef void (*dgus_anim_exec_cb_t)(void *var,
                                    int32_t value,
                                    void *user_data);

/* Called when an animation has completed all repeats/playback cycles. */
typedef void (*dgus_anim_ready_cb_t)(struct dgus_anim *anim,
                                     void *user_data);

/* Returns eased progress in the range 0..DGUS_ANIM_PROGRESS_MAX. */
typedef int32_t (*dgus_anim_path_cb_t)(const struct dgus_anim *anim);

/* Runtime state and configuration for one scalar animation. */
typedef struct dgus_anim {
    bool active;
    bool playback;
    bool reverse;
    void *var;
    void *user_data;
    dgus_anim_exec_cb_t exec_cb;
    dgus_anim_ready_cb_t ready_cb;
    dgus_anim_path_cb_t path_cb;
    int32_t start_value;
    int32_t end_value;
    int32_t current_value;
    uint32_t duration_ms;
    uint32_t delay_ms;
    uint32_t elapsed_ms;
    uint16_t repeat_count;
    uint16_t repeat_left;
} dgus_anim_t;

/* Fixed-capacity animation manager. The caller owns the backing array. */
typedef struct {
    dgus_anim_t *items;
    uint16_t capacity;
} dgus_anim_manager_t;

/* Callback used by the generic move animation executor. */
typedef dgus_status_t (*dgus_anim_move_set_pos_cb_t)(dgus_ui_t *ui,
                                                     void *target,
                                                     int16_t x,
                                                     int16_t y);

/* Context used by the default widget-move animation executor. */
typedef struct {
    dgus_ui_t *ui;
    void *target;
    dgus_anim_move_set_pos_cb_t set_pos_cb;
    int16_t start_x;
    int16_t start_y;
    int16_t end_x;
    int16_t end_y;
    int16_t last_x;
    int16_t last_y;
} dgus_anim_move_t;

/* Initialize an animation manager with a static animation pool. */
void dgus_anim_manager_init(dgus_anim_manager_t *manager,
                            dgus_anim_t *items,
                            uint16_t capacity);

/* Initialize an animation descriptor with conservative defaults. */
void dgus_anim_init(dgus_anim_t *anim);

/* Set the animated target pointer passed to the exec callback. */
void dgus_anim_set_var(dgus_anim_t *anim, void *var);

/* Set the start and end scalar values for the animation. */
void dgus_anim_set_values(dgus_anim_t *anim,
                          int32_t start_value,
                          int32_t end_value);

/* Set the animation duration in milliseconds. */
void dgus_anim_set_time(dgus_anim_t *anim, uint32_t duration_ms);

/* Set an optional start delay in milliseconds. */
void dgus_anim_set_delay(dgus_anim_t *anim, uint32_t delay_ms);

/* Set whether each repeated cycle should play in the opposite direction. */
void dgus_anim_set_playback(dgus_anim_t *anim, bool enabled);

/* Set the total number of cycles, or DGUS_ANIM_REPEAT_INFINITE. */
void dgus_anim_set_repeat_count(dgus_anim_t *anim, uint16_t repeat_count);

/* Set the callback that applies each interpolated value. */
void dgus_anim_set_exec_cb(dgus_anim_t *anim, dgus_anim_exec_cb_t cb);

/* Set the callback called after the animation is fully complete. */
void dgus_anim_set_ready_cb(dgus_anim_t *anim, dgus_anim_ready_cb_t cb);

/* Set the easing/path callback. NULL selects linear easing. */
void dgus_anim_set_path_cb(dgus_anim_t *anim, dgus_anim_path_cb_t cb);

/* Start an animation by copying the descriptor into the manager pool. */
dgus_status_t dgus_anim_start(dgus_anim_manager_t *manager,
                              const dgus_anim_t *template_anim);

/* Stop animations that match var and/or exec_cb; NULL matches any value. */
void dgus_anim_del(dgus_anim_manager_t *manager,
                   void *var,
                   dgus_anim_exec_cb_t exec_cb);

/* Advance every active animation by elapsed_ms and emit changed values. */
void dgus_anim_tick(dgus_anim_manager_t *manager, uint32_t elapsed_ms);

/* Return true when at least one animation in the manager is active. */
bool dgus_anim_has_active(const dgus_anim_manager_t *manager);

/* Return raw linear progress in the range 0..DGUS_ANIM_PROGRESS_MAX. */
int32_t dgus_anim_get_progress(const dgus_anim_t *anim);

/* Linear easing: constant speed. */
int32_t dgus_anim_path_linear(const dgus_anim_t *anim);

/* Quadratic ease-in: slow start, fast end. */
int32_t dgus_anim_path_ease_in(const dgus_anim_t *anim);

/* Quadratic ease-out: fast start, slow end. */
int32_t dgus_anim_path_ease_out(const dgus_anim_t *anim);

/* Quadratic ease-in-out: slow start, fast middle, slow end. */
int32_t dgus_anim_path_ease_in_out(const dgus_anim_t *anim);

/* Initialize a move animation with a widget-specific position setter. */
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
                         dgus_anim_path_cb_t path_cb);

/* Initialize a move animation for data variable display (type 0x10). */
void dgus_anim_data_var_move_init(dgus_anim_move_t *move,
                                  dgus_anim_t *anim,
                                  dgus_ui_t *ui,
                                  dgus_data_var_t *data,
                                  int16_t start_x,
                                  int16_t start_y,
                                  int16_t end_x,
                                  int16_t end_y,
                                  uint32_t duration_ms,
                                  dgus_anim_path_cb_t path_cb);

/* Initialize a move animation for text display (type 0x11). */
void dgus_anim_text_move_init(dgus_anim_move_t *move,
                              dgus_anim_t *anim,
                              dgus_ui_t *ui,
                              dgus_text_t *text,
                              int16_t start_x,
                              int16_t start_y,
                              int16_t end_x,
                              int16_t end_y,
                              uint32_t duration_ms,
                              dgus_anim_path_cb_t path_cb);

/* Initialize a move animation for variable icon display (type 0x00). */
void dgus_anim_var_icon_move_init(dgus_anim_move_t *move,
                                  dgus_anim_t *anim,
                                  dgus_ui_t *ui,
                                  dgus_var_icon_t *icon,
                                  int16_t start_x,
                                  int16_t start_y,
                                  int16_t end_x,
                                  int16_t end_y,
                                  uint32_t duration_ms,
                                  dgus_anim_path_cb_t path_cb);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_ANIM_H */
