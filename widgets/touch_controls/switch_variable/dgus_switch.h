#ifndef DGUS_SWITCH_H
#define DGUS_SWITCH_H

#include "dgus_obj.h"

#ifdef __cplusplus
extern "C" {
#endif

/* VP-backed switch helper. Display effects are defined by DGUS project data.
 *
 * Parameters:
 * - vp is the switch variable address. The DGUS project can bind icon/text
 *   behavior to the same VP, or the MCU can route uploads from this VP.
 * - off_value/on_value are the words written to vp for false/true states.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t off_value;
    uint16_t on_value;
    bool checked;
} dgus_switch_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_switch_t, dgus_switch);

/* Create a switch object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS VP storing the switch state.
 */
dgus_switch_t *dgus_switch_create(dgus_page_t *page,
                                  uint16_t id,
                                  uint16_t vp);

/* Configure switch values for off/on states.
 * off_value: word written when checked is false.
 * on_value: word written when checked is true.
 */
void dgus_switch_set_values(dgus_switch_t *sw,
                            uint16_t off_value,
                            uint16_t on_value);

/* Write a switch object's checked state.
 * checked: true writes on_value; false writes off_value.
 */
dgus_status_t dgus_switch_set_checked(dgus_ui_t *ui,
                                      dgus_switch_t *sw,
                                      bool checked);

#ifdef __cplusplus
}
#endif

#endif /* DGUS_SWITCH_H */
