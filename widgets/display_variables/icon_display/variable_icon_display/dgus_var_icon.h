#ifndef DGUS_VAR_ICON_H
#define DGUS_VAR_ICON_H

#include "dgus_display.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Offsets for variable icon display (14.BIN type 0x00). */
#define DGUS_VAR_ICON_SP_VP          0x00u
#define DGUS_VAR_ICON_SP_POS         0x01u
#define DGUS_VAR_ICON_SP_V_MIN       0x03u
#define DGUS_VAR_ICON_SP_V_MAX       0x04u
#define DGUS_VAR_ICON_SP_ICON_MIN    0x05u
#define DGUS_VAR_ICON_SP_ICON_MAX    0x06u
#define DGUS_VAR_ICON_SP_ICON_LIB    0x07u
#define DGUS_VAR_ICON_SP_LAYER       0x08u
#define DGUS_VAR_ICON_SP_GAMMA       0x09u

/* Variable icon display. The VP value selects the displayed icon.
 *
 * Parameters:
 * - vp is the runtime value address. Values between min_value and max_value map
 *   to icon IDs between min_icon and max_icon.
 * - sp is the descriptor base for type 0x00. Use DGUS_SP_NONE for VP-only
 *   writes; position/range/library/hide APIs then return DGUS_ERR_ARG.
 * - icon_lib is the DGUS icon library id; mode uses dgus_icon_mode_t.
 * - icon_gamma is the manual's brightness/gamma byte for icon blending.
 */
typedef struct {
    dgus_obj_t obj;
    uint16_t sp;
} dgus_var_icon_t;

DGUS_WIDGET_DECLARE_CALLBACKS(dgus_var_icon_t, dgus_var_icon);

/* Create a variable icon display object from the page pool.
 * page: target page storage.
 * id: host-side unique object id.
 * vp: DGUS value VP linked by the display variable.
 * sp: descriptor pointer for 14.BIN type 0x00, or DGUS_SP_NONE.
 */
dgus_var_icon_t *dgus_var_icon_create(dgus_page_t *page,
                                      uint16_t id,
                                      uint16_t vp,
                                      uint16_t sp);

/* Return true when the object was created with a descriptor pointer. */
bool dgus_var_icon_has_sp(const dgus_var_icon_t *icon);

/* Write the VP value that selects the displayed icon.
 * value: source value compared with the descriptor value range.
 */
dgus_status_t dgus_var_icon_set_value(dgus_ui_t *ui,
                                      const dgus_var_icon_t *icon,
                                      uint16_t value);

/* Write arbitrary descriptor fields for variable icon display.
 * offset: word offset from sp; prefer DGUS_VAR_ICON_SP_* constants.
 * value/words: raw descriptor data in DGUS word order.
 */
dgus_status_t dgus_var_icon_desc_write_u16(dgus_ui_t *ui,
                                           const dgus_var_icon_t *icon,
                                           uint16_t offset,
                                           uint16_t value);
dgus_status_t dgus_var_icon_desc_write_words(dgus_ui_t *ui,
                                             const dgus_var_icon_t *icon,
                                             uint16_t offset,
                                             const uint16_t *words,
                                             uint8_t word_count);

/* Change the linked VP at SP+0.
 * vp: linked data VP, or 0xFF00 to hide the display per the manual.
 */
dgus_status_t dgus_var_icon_set_desc_vp(dgus_ui_t *ui,
                                        const dgus_var_icon_t *icon,
                                        uint16_t vp);

/* Hide by writing 0xFF00 to SP+0. */
dgus_status_t dgus_var_icon_hide(dgus_ui_t *ui,
                                 const dgus_var_icon_t *icon);

/* Show by restoring a linked VP at SP+0. */
dgus_status_t dgus_var_icon_show(dgus_ui_t *ui,
                                 const dgus_var_icon_t *icon);

/* Move the icon by writing X,Y to SP+1.
 * x/y: top-left icon position in pixels.
 */
dgus_status_t dgus_var_icon_set_pos(dgus_ui_t *ui,
                                    const dgus_var_icon_t *icon,
                                    int16_t x,
                                    int16_t y);

/* Set value range fields.
 * min_value/max_value: inclusive VP value range used for icon selection.
 */
dgus_status_t dgus_var_icon_set_value_range(dgus_ui_t *ui,
                                            const dgus_var_icon_t *icon,
                                            uint16_t min_value,
                                            uint16_t max_value);
/* Set icon range fields.
 * min_icon/max_icon: inclusive icon ID range from the selected icon library.
 */
dgus_status_t dgus_var_icon_set_icon_range(dgus_ui_t *ui,
                                           const dgus_var_icon_t *icon,
                                           uint16_t min_icon,
                                           uint16_t max_icon);

/* Configure icon library and transparent/opaque mode at SP+7.
 * icon_lib: DGUS icon library id.
 * mode: DGUS_ICON_TRANSPARENT or DGUS_ICON_OPAQUE.
 */
dgus_status_t dgus_var_icon_set_library_mode(dgus_ui_t *ui,
                                             const dgus_var_icon_t *icon,
                                             uint8_t icon_lib,
                                             dgus_icon_mode_t mode);

/* Configure layer mode and icon gamma at SP+8.
 * layer: overwrite/blend mode from dgus_layer_mode_t.
 * icon_gamma: DGUS brightness/gamma byte, commonly 0x00..0xFF.
 */
dgus_status_t dgus_var_icon_set_layer(dgus_ui_t *ui,
                                      const dgus_var_icon_t *icon,
                                      dgus_layer_mode_t layer,
                                      uint8_t icon_gamma);

                                      


#ifdef __cplusplus
}
#endif

#endif /* DGUS_VAR_ICON_H */
