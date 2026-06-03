#ifndef DGUS_UI_INTERNAL_H
#define DGUS_UI_INTERNAL_H

#include <stddef.h>
#include <stdint.h>

#include "dgus_obj.h"

/* Allocate zeroed widget storage from a page-owned static pool. */
void *dgus_page_alloc(dgus_page_t *page, size_t size);

/* Dispatch one prepared UI event to object, page, then global callbacks. */
void dgus_ui_dispatch_event(dgus_ui_event_t *event);

/* Write bytes to an object VP with a widget-owned offset. */
dgus_status_t dgus_obj_write_bytes_at(dgus_ui_t *ui,
                                      const dgus_obj_t *obj,
                                      uint16_t offset,
                                      const uint8_t *data,
                                      uint16_t byte_len);

/* Write words to an object VP with a widget-owned offset. */
dgus_status_t dgus_obj_write_words_at(dgus_ui_t *ui,
                                      const dgus_obj_t *obj,
                                      uint16_t offset,
                                      const uint16_t *words,
                                      uint8_t word_count);

/* Write one word to an object VP with a widget-owned offset. */
dgus_status_t dgus_obj_write_u16_at(dgus_ui_t *ui,
                                    const dgus_obj_t *obj,
                                    uint16_t offset,
                                    uint16_t value);

/* Write bytes to a DGUS descriptor pointer field. */
dgus_status_t dgus_desc_write_bytes(dgus_ui_t *ui,
                                    uint16_t sp,
                                    uint16_t offset,
                                    const uint8_t *data,
                                    uint16_t byte_len);

/* Write bytes to a widget-owned DGUS descriptor field and notify the widget's
 * parameter callback after a successful write.
 */
dgus_status_t dgus_obj_desc_write_bytes_at(dgus_ui_t *ui,
                                           const dgus_obj_t *obj,
                                           uint16_t sp,
                                           uint16_t offset,
                                           const uint8_t *data,
                                           uint16_t byte_len);

/* Write one word to a DGUS descriptor pointer field. */
dgus_status_t dgus_desc_write_u16(dgus_ui_t *ui,
                                  uint16_t sp,
                                  uint16_t offset,
                                  uint16_t value);

/* Write one word to a widget-owned DGUS descriptor field and notify the
 * widget's parameter callback after a successful write.
 */
dgus_status_t dgus_obj_desc_write_u16_at(dgus_ui_t *ui,
                                         const dgus_obj_t *obj,
                                         uint16_t sp,
                                         uint16_t offset,
                                         uint16_t value);

/* Write multiple words to a DGUS descriptor pointer field. */
dgus_status_t dgus_desc_write_words(dgus_ui_t *ui,
                                    uint16_t sp,
                                    uint16_t offset,
                                    const uint16_t *words,
                                    uint8_t word_count);

/* Write multiple words to a widget-owned DGUS descriptor field and notify the
 * widget's parameter callback after a successful write.
 */
dgus_status_t dgus_obj_desc_write_words_at(dgus_ui_t *ui,
                                           const dgus_obj_t *obj,
                                           uint16_t sp,
                                           uint16_t offset,
                                           const uint16_t *words,
                                           uint8_t word_count);

/* Write an X,Y pair to a DGUS descriptor pointer field. */
dgus_status_t dgus_desc_write_point(dgus_ui_t *ui,
                                    uint16_t sp,
                                    uint16_t offset,
                                    dgus_point_t point);

/* Write an X,Y pair to a widget-owned DGUS descriptor field and notify the
 * widget's parameter callback after a successful write.
 */
dgus_status_t dgus_obj_desc_write_point_at(dgus_ui_t *ui,
                                           const dgus_obj_t *obj,
                                           uint16_t sp,
                                           uint16_t offset,
                                           dgus_point_t point);

/* Write an X0,Y0,X1,Y1 rectangle to a DGUS descriptor pointer field. */
dgus_status_t dgus_desc_write_rect(dgus_ui_t *ui,
                                   uint16_t sp,
                                   uint16_t offset,
                                   dgus_rect_t rect);

/* Write an X0,Y0,X1,Y1 rectangle to a widget-owned DGUS descriptor field and
 * notify the widget's parameter callback after a successful write.
 */
dgus_status_t dgus_obj_desc_write_rect_at(dgus_ui_t *ui,
                                          const dgus_obj_t *obj,
                                          uint16_t sp,
                                          uint16_t offset,
                                          dgus_rect_t rect);

/* Button event helper used by the UI router. */
dgus_status_t dgus_button_handle_value_upload(dgus_ui_t *ui,
                                              dgus_obj_t *obj,
                                              dgus_ui_event_t *event,
                                              uint16_t value);

/* Key-return group event helper used by the UI router. */
dgus_status_t dgus_key_return_group_handle_value_upload(dgus_obj_t *obj,
                                                        dgus_ui_event_t *event,
                                                        uint16_t value);

#endif /* DGUS_UI_INTERNAL_H */
