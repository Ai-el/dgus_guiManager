#ifndef DGUS_DISPLAY_H
#define DGUS_DISPLAY_H

#include "dgus_obj.h"

#ifdef __cplusplus
extern "C" {
#endif

/* DGUS display-variable command codes from 14.BIN chapter 7. */
#define DGUS_DVAR_VAR_ICON             0x00u
#define DGUS_DVAR_ANIM_ICON            0x01u
#define DGUS_DVAR_SLIDER_INDICATOR     0x02u
#define DGUS_DVAR_ART_TEXT             0x03u
#define DGUS_DVAR_PIC_ANIM             0x04u
#define DGUS_DVAR_ICON_ROTATE          0x05u
#define DGUS_DVAR_BIT_ICON             0x06u
#define DGUS_DVAR_JPEG_PAN             0x07u
#define DGUS_DVAR_JPEG                 0x08u
#define DGUS_DVAR_BATCH_ICON           0x09u
#define DGUS_DVAR_DATA_VAR             0x10u
#define DGUS_DVAR_TEXT                 0x11u
#define DGUS_DVAR_RTC                  0x12u
#define DGUS_DVAR_HEX                  0x13u
#define DGUS_DVAR_TEXT_SCROLL          0x14u
#define DGUS_DVAR_DATA_WINDOW          0x15u
#define DGUS_DVAR_DGUSII_TEXT          0x16u
#define DGUS_DVAR_ICON_ROLLER          0x17u
#define DGUS_DVAR_GTF_TEXT             0x18u
#define DGUS_DVAR_TREND_CURVE          0x20u
#define DGUS_DVAR_BASIC_GRAPH          0x21u
#define DGUS_DVAR_PROGRESS             0x23u
#define DGUS_DVAR_AREA_SCROLL          0x24u
#define DGUS_DVAR_QR                   0x25u
#define DGUS_DVAR_AREA_BRIGHTNESS      0x26u
#define DGUS_DVAR_DATA_TRANSFER        0x30u
#define DGUS_DVAR_VIDEO                0x31u

/* Icon background mode fields used by icon-like widgets:
 * DGUS_ICON_TRANSPARENT keeps the source icon transparent pixels transparent;
 * DGUS_ICON_OPAQUE draws the whole icon rectangle.
 */
typedef enum {
    DGUS_ICON_TRANSPARENT = 0,
    DGUS_ICON_OPAQUE = 1
} dgus_icon_mode_t;

/* Descriptor layer/blending mode used by icon widgets that expose SP layer
 * fields. Exact visual output still depends on the DGUS project's icon data.
 */
typedef enum {
    DGUS_LAYER_OVERWRITE = 0,
    DGUS_LAYER_BLEND_1 = 1,
    DGUS_LAYER_BLEND_2 = 2
} dgus_layer_mode_t;

/* Numeric data encoding used by data-variable style widgets. These values map
 * directly to the DGUS manual's data-mode field, so choose the mode that
 * matches how the VP data is stored in the screen project.
 */
typedef enum {
    DGUS_DATA_WORD = 0,
    DGUS_DATA_WORD_HIGH_BYTE = 1,
    DGUS_DATA_WORD_LOW_BYTE = 2,
    DGUS_DATA_LONG = 3,
    DGUS_DATA_DLONG = 4,
    DGUS_DATA_UWORD = 5,
    DGUS_DATA_ULONG = 6
} dgus_data_mode_t;

/* Text/numeric alignment used by data-variable digit descriptors. */
typedef enum {
    DGUS_ALIGN_LEFT = 0,
    DGUS_ALIGN_RIGHT = 1
} dgus_align_t;

/* Progress fill direction in screen coordinates. */
typedef enum {
    DGUS_PROGRESS_RIGHT = 0,
    DGUS_PROGRESS_LEFT = 1,
    DGUS_PROGRESS_UP = 2,
    DGUS_PROGRESS_DOWN = 3
} dgus_progress_dir_t;

/* Area scroll direction in screen coordinates. */
typedef enum {
    DGUS_SCROLL_LEFT = 0,
    DGUS_SCROLL_RIGHT = 1,
    DGUS_SCROLL_UP = 2,
    DGUS_SCROLL_DOWN = 3
} dgus_scroll_dir_t;

#ifdef __cplusplus
}
#endif

#endif /* DGUS_DISPLAY_H */
