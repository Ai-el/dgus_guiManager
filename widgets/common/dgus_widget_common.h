#ifndef DGUS_WIDGET_COMMON_H
#define DGUS_WIDGET_COMMON_H

#include "../../src/dgus_ui_internal.h"

void dgus_widget_init_obj(dgus_obj_t *obj,
                          uint16_t id,
                          dgus_obj_type_t type,
                          uint16_t vp,
                          uint8_t word_count);

uint16_t dgus_pack_u8(uint8_t high, uint8_t low);

bool dgus_widget_has_sp(uint16_t sp);

#endif /* DGUS_WIDGET_COMMON_H */
