#include "dgus_widget_common.h"

void dgus_widget_init_obj(dgus_obj_t *obj,
                          uint16_t id,
                          dgus_obj_type_t type,
                          uint16_t vp,
                          uint8_t word_count)
{
    dgus_obj_init(obj, id, type, vp, word_count, NULL, NULL);
}

uint16_t dgus_pack_u8(uint8_t high, uint8_t low)
{
    return (uint16_t)(((uint16_t)high << 8) | (uint16_t)low);
}

bool dgus_widget_has_sp(uint16_t sp)
{
    return sp != DGUS_SP_NONE;
}
