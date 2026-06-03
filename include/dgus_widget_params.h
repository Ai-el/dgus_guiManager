#ifndef DGUS_WIDGET_PARAMS_H
#define DGUS_WIDGET_PARAMS_H

/* Common widget API parameter rules.
 *
 * page:
 *   Page descriptor initialized by dgus_page_init(). Create functions allocate
 *   the widget object from page->memory and add its base dgus_obj_t to
 *   page->objects. The caller owns both arrays; this library does not malloc.
 *
 * id:
 *   Host-side object identifier used by dgus_page_find_obj() and event logs.
 *   It only needs to be unique within the page and does not need to match a
 *   DGUS PC-project control number.
 *
 * vp:
 *   DGUS variable pointer for runtime data. Use the VP configured in the DGUS
 *   PC project for the matching display or touch control. DGUS_VP_NONE is for
 *   widgets that have no runtime VP; VP write/read functions return
 *   DGUS_ERR_ARG when vp is DGUS_VP_NONE.
 *
 * sp:
 *   DGUS display descriptor pointer for SP fields in the 14.BIN manual table.
 *   Use DGUS_SP_NONE when the MCU must not edit descriptor fields. In that
 *   mode dgus_xxx_has_sp() returns false and descriptor APIs return
 *   DGUS_ERR_ARG, while VP data APIs still work.
 *
 * ui:
 *   UI manager initialized by dgus_ui_init(). Write/read functions use
 *   ui->dev to transmit 0x82/0x83 frames, so both ui and ui->dev must be
 *   valid.
 *
 * offset:
 *   Descriptor or VP word offset from sp or vp, not a byte offset. Prefer the
 *   named DGUS_xxx_SP_* and DGUS_xxx_VP_* constants from the widget header.
 *
 * x/y, rect, width/height:
 *   Screen coordinates and sizes in pixels. DGUS coordinates grow right and
 *   down from the top-left corner.
 *
 * rgb565:
 *   16-bit RGB565 color, for example 0xF800 red, 0x07E0 green, 0x001F blue,
 *   and 0xFFFF white.
 *
 * word_count/byte_len/max_chars/max_bytes:
 *   These must fit both DGUS_TX_BUFFER_SIZE and the VP/SP space allocated in
 *   the DGUS project. ASCII text helpers write one DGUS word per character.
 */

#endif /* DGUS_WIDGET_PARAMS_H */
