#include "dgus_ui_internal.h"

#include <stddef.h>
#include <string.h>

static size_t dgus_align_size(size_t value)
{
    const size_t align = sizeof(void *);

    return (value + (align - 1u)) & ~(align - 1u);
}

void dgus_ui_dispatch_event(dgus_ui_event_t *event)
{
    if ((event->target != NULL) && (event->target->event_cb != NULL))
    {
        event->target->event_cb(event, event->target->user_data);
    }
    if ((event->page != NULL) && (event->page->event_cb != NULL))
    {
        event->page->event_cb(event, event->page->user_data);
    }
    if ((event->ui != NULL) && (event->ui->event_cb != NULL))
    {
        event->ui->event_cb(event, event->ui->user_data);
    }
}

static void dgus_obj_dispatch_param_changed(dgus_ui_t *ui,
                                            const dgus_obj_t *obj,
                                            dgus_widget_param_space_t space,
                                            uint16_t address,
                                            uint16_t offset,
                                            const uint8_t *bytes,
                                            uint16_t byte_len)
{
    dgus_widget_param_event_t event;

    if ((obj == NULL) || (obj->param_changed_cb == NULL))
    {
        return;
    }

    event.ui = ui;
    event.target = obj;
    event.space = space;
    event.address = address;
    event.offset = offset;
    event.byte_len = byte_len;
    event.bytes = bytes;
    event.value = ((bytes != NULL) && (byte_len >= 2u)) ? dgus_get_u16_be(bytes) : 0u;

    obj->param_changed_cb(&event, obj->param_changed_user_data);
}

uint8_t dgus_obj_get_word_count(const dgus_obj_t *obj)
{
    if ((obj == NULL) || (obj->word_count == 0u))
    {
        return 1u;
    }

    return obj->word_count;
}

void dgus_obj_init(dgus_obj_t *obj,
                   uint16_t id,
                   dgus_obj_type_t type,
                   uint16_t vp,
                   uint8_t word_count,
                   dgus_ui_event_cb_t cb,
                   void *user_data)
{
    if (obj == NULL)
    {
        return;
    }

    obj->id = id;
    obj->type = type;
    obj->vp = vp;
    obj->word_count = word_count;
    obj->flags = 0u;
    obj->event_cb = cb;
    obj->user_data = user_data;
    obj->param_changed_cb = NULL;
    obj->param_changed_user_data = NULL;
}

void dgus_obj_set_flags(dgus_obj_t *obj, uint16_t flags)
{
    if (obj != NULL)
    {
        obj->flags = flags;
    }
}

void dgus_obj_set_vp(dgus_obj_t *obj, uint16_t vp)
{
    if (obj != NULL)
    {
        obj->vp = vp;
    }
}

void dgus_obj_set_word_count(dgus_obj_t *obj, uint8_t word_count)
{
    if (obj != NULL)
    {
        obj->word_count = word_count;
    }
}

void dgus_obj_set_event_callback(dgus_obj_t *obj,
                                 dgus_ui_event_cb_t cb,
                                 void *user_data)
{
    if (obj != NULL)
    {
        obj->event_cb = cb;
        obj->user_data = user_data;
    }
}

void dgus_obj_set_trigger_callback(dgus_obj_t *obj,
                                   dgus_ui_event_cb_t cb,
                                   void *user_data)
{
    dgus_obj_set_event_callback(obj, cb, user_data);
}

void dgus_obj_set_param_changed_callback(dgus_obj_t *obj,
                                         dgus_widget_param_cb_t cb,
                                         void *user_data)
{
    if (obj != NULL)
    {
        obj->param_changed_cb = cb;
        obj->param_changed_user_data = user_data;
    }
}

void dgus_page_init(dgus_page_t *page,
                    uint16_t id,
                    const char *name,
                    dgus_obj_t **objects,
                    uint16_t object_capacity,
                    void *memory,
                    size_t memory_size,
                    dgus_ui_event_cb_t cb,
                    void *user_data)
{
    if (page == NULL)
    {
        return;
    }

    page->id = id;
    page->name = name;
    page->objects = objects;
    page->object_count = 0u;
    page->object_capacity = object_capacity;
    page->memory = (uint8_t *)memory;
    page->memory_size = memory_size;
    page->memory_used = 0u;
    page->event_cb = cb;
    page->user_data = user_data;
    page->load_cb = NULL;
    page->load_user_data = NULL;
    page->unload_cb = NULL;
    page->unload_user_data = NULL;
}

dgus_status_t dgus_page_add_obj(dgus_page_t *page, dgus_obj_t *obj)
{
    if ((page == NULL) || (obj == NULL) || (page->objects == NULL))
    {
        return DGUS_ERR_ARG;
    }
    if (page->object_count >= page->object_capacity)
    {
        return DGUS_ERR_OVERFLOW;
    }

    page->objects[page->object_count] = obj;
    page->object_count++;

    return DGUS_OK;
}

void dgus_page_set_event_callback(dgus_page_t *page,
                                  dgus_ui_event_cb_t cb,
                                  void *user_data)
{
    if (page != NULL)
    {
        page->event_cb = cb;
        page->user_data = user_data;
    }
}

void dgus_page_set_load_callback(dgus_page_t *page,
                                 dgus_ui_simple_cb_t cb,
                                 void *user_data)
{
    if (page != NULL)
    {
        page->load_cb = cb;
        page->load_user_data = user_data;
    }
}

void dgus_page_set_unload_callback(dgus_page_t *page,
                                   dgus_ui_simple_cb_t cb,
                                   void *user_data)
{
    if (page != NULL)
    {
        page->unload_cb = cb;
        page->unload_user_data = user_data;
    }
}

void *dgus_page_alloc(dgus_page_t *page, size_t size)
{
    size_t aligned_used;
    void *ptr;

    if ((page == NULL) || (page->memory == NULL) || (size == 0u))
    {
        return NULL;
    }

    aligned_used = dgus_align_size(page->memory_used);
    if ((aligned_used > page->memory_size) ||
        (size > (page->memory_size - aligned_used)))
    {
        return NULL;
    }

    ptr = &page->memory[aligned_used];
    memset(ptr, 0, size);
    page->memory_used = aligned_used + dgus_align_size(size);

    return ptr;
}

dgus_obj_t *dgus_obj_create(dgus_page_t *page,
                            uint16_t id,
                            uint16_t vp,
                            uint8_t word_count)
{
    dgus_obj_t *obj;

    if ((page == NULL) || (page->object_count >= page->object_capacity))
    {
        return NULL;
    }
    obj = (dgus_obj_t *)dgus_page_alloc(page, sizeof(dgus_obj_t));
    if (obj == NULL)
    {
        return NULL;
    }
    dgus_obj_init(obj, id, DGUS_OBJ_CUSTOM, vp, word_count, NULL, NULL);
    if (dgus_page_add_obj(page, obj) != DGUS_OK)
    {
        return NULL;
    }

    return obj;
}

void dgus_ui_init(dgus_ui_t *ui,
                  dgus_t *dev,
                  dgus_page_t *pages,
                  uint16_t page_count)
{
    if (ui == NULL)
    {
        return;
    }

    ui->dev = dev;
    ui->pages = pages;
    ui->page_count = page_count;
    ui->active_page = NULL;
    ui->event_cb = NULL;
    ui->user_data = NULL;
}

void dgus_ui_set_event_callback(dgus_ui_t *ui,
                                dgus_ui_event_cb_t cb,
                                void *user_data)
{
    if (ui != NULL)
    {
        ui->event_cb = cb;
        ui->user_data = user_data;
    }
}

dgus_page_t *dgus_ui_find_page(dgus_ui_t *ui, uint16_t page_id)
{
    uint16_t i;

    if ((ui == NULL) || (ui->pages == NULL))
    {
        return NULL;
    }

    for (i = 0u; i < ui->page_count; i++)
    {
        if (ui->pages[i].id == page_id)
        {
            return &ui->pages[i];
        }
    }

    return NULL;
}

dgus_obj_t *dgus_page_find_obj(dgus_page_t *page, uint16_t obj_id)
{
    uint16_t i;

    if ((page == NULL) || (page->objects == NULL))
    {
        return NULL;
    }

    for (i = 0u; i < page->object_count; i++)
    {
        if ((page->objects[i] != NULL) && (page->objects[i]->id == obj_id))
        {
            return page->objects[i];
        }
    }

    return NULL;
}

static dgus_obj_t *dgus_page_find_obj_by_vp(dgus_page_t *page, uint16_t vp)
{
    uint16_t i;

    if ((page == NULL) || (page->objects == NULL))
    {
        return NULL;
    }

    for (i = 0u; i < page->object_count; i++)
    {
        dgus_obj_t *obj = page->objects[i];
        uint8_t words;
        uint16_t end;

        if (obj == NULL)
        {
            continue;
        }
        if (obj->vp == DGUS_VP_NONE)
        {
            continue;
        }

        words = dgus_obj_get_word_count(obj);
        end = (uint16_t)(obj->vp + words);
        if ((vp >= obj->vp) && (vp < end))
        {
            return obj;
        }
    }

    return NULL;
}

dgus_obj_t *dgus_ui_find_obj_by_vp(dgus_ui_t *ui, uint16_t vp)
{
    uint16_t i;

    if (ui == NULL)
    {
        return NULL;
    }

    if (ui->active_page != NULL)
    {
        dgus_obj_t *obj = dgus_page_find_obj_by_vp(ui->active_page, vp);
        if (obj != NULL)
        {
            return obj;
        }
    }

    for (i = 0u; i < ui->page_count; i++)
    {
        dgus_obj_t *obj = dgus_page_find_obj_by_vp(&ui->pages[i], vp);
        if ((obj != NULL) && ((obj->flags & DGUS_UI_OBJ_F_GLOBAL) != 0u))
        {
            return obj;
        }
    }

    if (ui->active_page == NULL)
    {
        for (i = 0u; i < ui->page_count; i++)
        {
            dgus_obj_t *obj = dgus_page_find_obj_by_vp(&ui->pages[i], vp);
            if (obj != NULL)
            {
                return obj;
            }
        }
    }

    return NULL;
}

static void dgus_ui_set_active_page_local(dgus_ui_t *ui, dgus_page_t *page)
{
    dgus_ui_event_t event;

    if ((ui == NULL) || (ui->active_page == page))
    {
        return;
    }

    if (ui->active_page != NULL)
    {
        event.code = DGUS_UI_EVENT_PAGE_UNLOAD;
        event.ui = ui;
        event.page = ui->active_page;
        event.target = NULL;
        event.vp = DGUS_SYS_ADDR_CURRENT_PAGE;
        event.word_count = 0u;
        event.word_data = NULL;
        event.value = ui->active_page->id;
        event.touch_state = 0u;
        event.x = 0u;
        event.y = 0u;
        event.frame = NULL;
        if (ui->active_page->unload_cb != NULL)
        {
            ui->active_page->unload_cb(ui->active_page->unload_user_data);
        }
        dgus_ui_dispatch_event(&event);
    }

    ui->active_page = page;

    if (ui->active_page != NULL)
    {
        event.code = DGUS_UI_EVENT_PAGE_LOAD;
        event.ui = ui;
        event.page = ui->active_page;
        event.target = NULL;
        event.vp = DGUS_SYS_ADDR_CURRENT_PAGE;
        event.word_count = 0u;
        event.word_data = NULL;
        event.value = ui->active_page->id;
        event.touch_state = 0u;
        event.x = 0u;
        event.y = 0u;
        event.frame = NULL;
        if (ui->active_page->load_cb != NULL)
        {
            ui->active_page->load_cb(ui->active_page->load_user_data);
        }
        dgus_ui_dispatch_event(&event);
    }
}

dgus_status_t dgus_ui_set_page(dgus_ui_t *ui, uint16_t page_id)
{
    dgus_page_t *page;
    dgus_status_t status;

    if ((ui == NULL) || (ui->dev == NULL))
    {
        return DGUS_ERR_ARG;
    }

    page = dgus_ui_find_page(ui, page_id);
    if (page == NULL)
    {
        return DGUS_ERR_ARG;
    }

    status = dgus_set_page(ui->dev, page_id);
    if (status == DGUS_OK)
    {
        dgus_ui_set_active_page_local(ui, page);
    }

    return status;
}

dgus_status_t dgus_ui_request_active_page(dgus_ui_t *ui)
{
    if ((ui == NULL) || (ui->dev == NULL))
    {
        return DGUS_ERR_ARG;
    }

    return dgus_request_current_page(ui->dev);
}

static void dgus_ui_dispatch_write_ok(dgus_ui_t *ui, const dgus_frame_t *frame)
{
    dgus_ui_event_t event;

    event.code = DGUS_UI_EVENT_WRITE_OK;
    event.ui = ui;
    event.page = (ui != NULL) ? ui->active_page : NULL;
    event.target = NULL;
    event.vp = 0u;
    event.word_count = 0u;
    event.word_data = NULL;
    event.value = 0u;
    event.touch_state = 0u;
    event.x = 0u;
    event.y = 0u;
    event.frame = frame;
    dgus_ui_dispatch_event(&event);
}

static dgus_status_t dgus_ui_dispatch_value(dgus_ui_t *ui,
                                            const dgus_frame_t *frame,
                                            uint16_t vp,
                                            uint8_t word_count,
                                            const uint8_t *word_data)
{
    dgus_ui_event_t event;
    dgus_obj_t *obj;
    dgus_page_t *page;
    uint16_t value = 0u;

    obj = dgus_ui_find_obj_by_vp(ui, vp);
    page = (ui != NULL) ? ui->active_page : NULL;

    if ((word_count > 0u) && (word_data != NULL))
    {
        value = dgus_get_u16_be(word_data);
    }

    event.code = DGUS_UI_EVENT_VALUE_CHANGED;
    event.ui = ui;
    event.page = page;
    event.target = obj;
    event.vp = vp;
    event.word_count = word_count;
    event.word_data = word_data;
    event.value = value;
    event.touch_state = 0u;
    event.x = 0u;
    event.y = 0u;
    event.frame = frame;
    dgus_ui_dispatch_event(&event);

    if ((obj != NULL) && (obj->type == DGUS_OBJ_BUTTON) && (word_count > 0u))
    {
        return dgus_button_handle_value_upload(ui, obj, &event, value);
    }
    if ((obj != NULL) && (obj->type == DGUS_OBJ_KEY_RETURN_GROUP) &&
        (word_count > 0u))
    {
        return dgus_key_return_group_handle_value_upload(obj, &event, value);
    }

    return DGUS_OK;
}

static void dgus_ui_dispatch_touch(dgus_ui_t *ui,
                                   const dgus_frame_t *frame,
                                   uint16_t vp,
                                   uint8_t word_count,
                                   const uint8_t *word_data)
{
    dgus_ui_event_t event;
    uint8_t state;

    if ((word_count < 3u) || (word_data == NULL) || (word_data[0] != 0x5Au))
    {
        return;
    }

    state = word_data[1];
    if (state == 0x01u)
    {
        event.code = DGUS_UI_EVENT_TOUCH_PRESSED;
    }
    else if (state == 0x02u)
    {
        event.code = DGUS_UI_EVENT_TOUCH_RELEASED;
    }
    else if (state == 0x03u)
    {
        event.code = DGUS_UI_EVENT_TOUCH_PRESSING;
    }
    else
    {
        return;
    }

    event.ui = ui;
    event.page = (ui != NULL) ? ui->active_page : NULL;
    event.target = dgus_ui_find_obj_by_vp(ui, vp);
    event.vp = vp;
    event.word_count = word_count;
    event.word_data = word_data;
    event.value = 0u;
    event.touch_state = state;
    event.x = dgus_get_u16_be(&word_data[2]);
    event.y = dgus_get_u16_be(&word_data[4]);
    event.frame = frame;
    dgus_ui_dispatch_event(&event);
}

dgus_status_t dgus_ui_process_frame(dgus_ui_t *ui,
                                    const dgus_frame_t *frame)
{
    uint16_t vp;
    uint8_t word_count;
    const uint8_t *word_data;

    if ((ui == NULL) || (frame == NULL))
    {
        return DGUS_ERR_ARG;
    }

    if (dgus_is_write_ok_frame(frame))
    {
        dgus_ui_dispatch_write_ok(ui, frame);
        return DGUS_OK;
    }

    if (!dgus_decode_var_frame(frame, &vp, &word_count, &word_data))
    {
        dgus_ui_event_t event;

        event.code = DGUS_UI_EVENT_FRAME;
        event.ui = ui;
        event.page = ui->active_page;
        event.target = NULL;
        event.vp = 0u;
        event.word_count = 0u;
        event.word_data = NULL;
        event.value = 0u;
        event.touch_state = 0u;
        event.x = 0u;
        event.y = 0u;
        event.frame = frame;
        dgus_ui_dispatch_event(&event);
        return DGUS_OK;
    }

    if ((vp == DGUS_SYS_ADDR_CURRENT_PAGE) &&
        (word_count > 0u) &&
        (word_data != NULL))
    {
        uint16_t page_id = dgus_get_u16_be(word_data);
        dgus_page_t *page = dgus_ui_find_page(ui, page_id);
        if (page != NULL)
        {
            dgus_ui_set_active_page_local(ui, page);
        }
    }

    if (vp == DGUS_SYS_ADDR_TOUCH_STATE)
    {
        dgus_ui_dispatch_touch(ui, frame, vp, word_count, word_data);
    }

    return dgus_ui_dispatch_value(ui, frame, vp, word_count, word_data);
}

void dgus_ui_frame_callback(dgus_t *dev,
                            const dgus_frame_t *frame,
                            void *user_data)
{
    (void)dev;
    (void)dgus_ui_process_frame((dgus_ui_t *)user_data, frame);
}

dgus_status_t dgus_obj_set_u16(dgus_ui_t *ui,
                               const dgus_obj_t *obj,
                               uint16_t value)
{
    dgus_status_t status;
    uint8_t data[2];

    if ((ui == NULL) || (ui->dev == NULL) || (obj == NULL) ||
        (obj->vp == DGUS_VP_NONE))
    {
        return DGUS_ERR_ARG;
    }

    status = dgus_write_u16(ui->dev, obj->vp, value);
    if (status == DGUS_OK)
    {
        dgus_put_u16_be(data, value);
        dgus_obj_dispatch_param_changed(ui,
                                        obj,
                                        DGUS_WIDGET_PARAM_VP,
                                        obj->vp,
                                        0u,
                                        data,
                                        sizeof(data));
    }

    return status;
}

dgus_status_t dgus_obj_set_i16(dgus_ui_t *ui,
                               const dgus_obj_t *obj,
                               int16_t value)
{
    return dgus_obj_set_u16(ui, obj, (uint16_t)value);
}

dgus_status_t dgus_obj_set_u32(dgus_ui_t *ui,
                               const dgus_obj_t *obj,
                               uint32_t value)
{
    dgus_status_t status;
    uint8_t data[4];

    if ((ui == NULL) || (ui->dev == NULL) || (obj == NULL) ||
        (obj->vp == DGUS_VP_NONE))
    {
        return DGUS_ERR_ARG;
    }

    status = dgus_write_u32(ui->dev, obj->vp, value);
    if (status == DGUS_OK)
    {
        dgus_put_u16_be(&data[0], (uint16_t)(value >> 16));
        dgus_put_u16_be(&data[2], (uint16_t)value);
        dgus_obj_dispatch_param_changed(ui,
                                        obj,
                                        DGUS_WIDGET_PARAM_VP,
                                        obj->vp,
                                        0u,
                                        data,
                                        sizeof(data));
    }

    return status;
}

dgus_status_t dgus_obj_read(dgus_ui_t *ui, const dgus_obj_t *obj)
{
    if ((ui == NULL) || (ui->dev == NULL) || (obj == NULL) ||
        (obj->vp == DGUS_VP_NONE))
    {
        return DGUS_ERR_ARG;
    }

    return dgus_read_words(ui->dev, obj->vp, dgus_obj_get_word_count(obj));
}

dgus_status_t dgus_obj_set_text_ascii(dgus_ui_t *ui,
                                      const dgus_obj_t *obj,
                                      const char *text,
                                      uint8_t max_chars,
                                      bool terminate)
{
    uint8_t data[DGUS_TX_BUFFER_SIZE];
    uint16_t data_len = 0u;
    uint8_t limit;
    uint8_t i;

    if ((ui == NULL) || (ui->dev == NULL) || (obj == NULL) ||
        (obj->vp == DGUS_VP_NONE) || (text == NULL))
    {
        return DGUS_ERR_ARG;
    }

    if (max_chars != 0u)
    {
        limit = max_chars;
    }
    else if (obj->word_count > 1u)
    {
        limit = (uint8_t)(obj->word_count - (terminate ? 1u : 0u));
    }
    else
    {
        return DGUS_ERR_ARG;
    }

    for (i = 0u; (i < limit) && (text[i] != '\0'); i++)
    {
        if ((uint16_t)(data_len + 2u) > (uint16_t)sizeof(data))
        {
            return DGUS_ERR_OVERFLOW;
        }
        data[data_len] = 0x00u;
        data[data_len + 1u] = (uint8_t)text[i];
        data_len = (uint16_t)(data_len + 2u);
    }

    if (terminate)
    {
        if ((uint16_t)(data_len + 2u) > (uint16_t)sizeof(data))
        {
            return DGUS_ERR_OVERFLOW;
        }
        data[data_len] = 0xFFu;
        data[data_len + 1u] = 0xFFu;
        data_len = (uint16_t)(data_len + 2u);
    }

    if (data_len == 0u)
    {
        return DGUS_ERR_ARG;
    }

    {
        dgus_status_t status = dgus_write_bytes(ui->dev,
                                                obj->vp,
                                                data,
                                                data_len);
        if (status == DGUS_OK)
        {
            dgus_obj_dispatch_param_changed(ui,
                                            obj,
                                            DGUS_WIDGET_PARAM_VP,
                                            obj->vp,
                                            0u,
                                            data,
                                            data_len);
        }

        return status;
    }
}

dgus_status_t dgus_obj_write_bytes(dgus_ui_t *ui,
                                   const dgus_obj_t *obj,
                                   const uint8_t *data,
                                   uint16_t byte_len)
{
    dgus_status_t status;

    if ((ui == NULL) || (ui->dev == NULL) || (obj == NULL) ||
        (obj->vp == DGUS_VP_NONE) || (data == NULL) || (byte_len == 0u))
    {
        return DGUS_ERR_ARG;
    }

    status = dgus_write_bytes(ui->dev, obj->vp, data, byte_len);
    if (status == DGUS_OK)
    {
        dgus_obj_dispatch_param_changed(ui,
                                        obj,
                                        DGUS_WIDGET_PARAM_VP,
                                        obj->vp,
                                        0u,
                                        data,
                                        byte_len);
    }

    return status;
}

dgus_status_t dgus_obj_write_words(dgus_ui_t *ui,
                                   const dgus_obj_t *obj,
                                   const uint16_t *words,
                                   uint8_t word_count)
{
    dgus_status_t status;
    uint8_t data[DGUS_TX_BUFFER_SIZE];
    uint16_t byte_len;
    uint8_t i;

    if ((ui == NULL) || (ui->dev == NULL) || (obj == NULL) ||
        (obj->vp == DGUS_VP_NONE) || (words == NULL) || (word_count == 0u))
    {
        return DGUS_ERR_ARG;
    }

    byte_len = (uint16_t)(word_count * 2u);
    if (byte_len > (uint16_t)sizeof(data))
    {
        return DGUS_ERR_OVERFLOW;
    }

    status = dgus_write_words(ui->dev, obj->vp, words, word_count);
    if (status == DGUS_OK)
    {
        for (i = 0u; i < word_count; i++)
        {
            dgus_put_u16_be(&data[(uint16_t)i * 2u], words[i]);
        }
        dgus_obj_dispatch_param_changed(ui,
                                        obj,
                                        DGUS_WIDGET_PARAM_VP,
                                        obj->vp,
                                        0u,
                                        data,
                                        byte_len);
    }

    return status;
}

dgus_status_t dgus_desc_write_bytes(dgus_ui_t *ui,
                                    uint16_t sp,
                                    uint16_t offset,
                                    const uint8_t *data,
                                    uint16_t byte_len)
{
    if ((ui == NULL) || (ui->dev == NULL) || (sp == DGUS_SP_NONE) ||
        (data == NULL) || (byte_len == 0u))
    {
        return DGUS_ERR_ARG;
    }

    return dgus_write_bytes(ui->dev, (uint16_t)(sp + offset), data, byte_len);
}

dgus_status_t dgus_obj_write_bytes_at(dgus_ui_t *ui,
                                      const dgus_obj_t *obj,
                                      uint16_t offset,
                                      const uint8_t *data,
                                      uint16_t byte_len)
{
    dgus_status_t status;
    uint16_t address;

    if ((ui == NULL) || (ui->dev == NULL) || (obj == NULL) ||
        (obj->vp == DGUS_VP_NONE) || (data == NULL) || (byte_len == 0u))
    {
        return DGUS_ERR_ARG;
    }

    address = (uint16_t)(obj->vp + offset);
    status = dgus_write_bytes(ui->dev, address, data, byte_len);
    if (status == DGUS_OK)
    {
        dgus_obj_dispatch_param_changed(ui,
                                        obj,
                                        DGUS_WIDGET_PARAM_VP,
                                        address,
                                        offset,
                                        data,
                                        byte_len);
    }

    return status;
}

dgus_status_t dgus_obj_write_words_at(dgus_ui_t *ui,
                                      const dgus_obj_t *obj,
                                      uint16_t offset,
                                      const uint16_t *words,
                                      uint8_t word_count)
{
    dgus_status_t status;
    uint8_t data[DGUS_TX_BUFFER_SIZE];
    uint16_t byte_len;
    uint16_t address;
    uint8_t i;

    if ((ui == NULL) || (ui->dev == NULL) || (obj == NULL) ||
        (obj->vp == DGUS_VP_NONE) || (words == NULL) || (word_count == 0u))
    {
        return DGUS_ERR_ARG;
    }

    byte_len = (uint16_t)(word_count * 2u);
    if (byte_len > (uint16_t)sizeof(data))
    {
        return DGUS_ERR_OVERFLOW;
    }

    address = (uint16_t)(obj->vp + offset);
    status = dgus_write_words(ui->dev, address, words, word_count);
    if (status == DGUS_OK)
    {
        for (i = 0u; i < word_count; i++)
        {
            dgus_put_u16_be(&data[(uint16_t)i * 2u], words[i]);
        }
        dgus_obj_dispatch_param_changed(ui,
                                        obj,
                                        DGUS_WIDGET_PARAM_VP,
                                        address,
                                        offset,
                                        data,
                                        byte_len);
    }

    return status;
}

dgus_status_t dgus_obj_write_u16_at(dgus_ui_t *ui,
                                    const dgus_obj_t *obj,
                                    uint16_t offset,
                                    uint16_t value)
{
    dgus_status_t status;
    uint8_t data[2];
    uint16_t address;

    if ((ui == NULL) || (ui->dev == NULL) || (obj == NULL) ||
        (obj->vp == DGUS_VP_NONE))
    {
        return DGUS_ERR_ARG;
    }

    address = (uint16_t)(obj->vp + offset);
    status = dgus_write_u16(ui->dev, address, value);
    if (status == DGUS_OK)
    {
        dgus_put_u16_be(data, value);
        dgus_obj_dispatch_param_changed(ui,
                                        obj,
                                        DGUS_WIDGET_PARAM_VP,
                                        address,
                                        offset,
                                        data,
                                        sizeof(data));
    }

    return status;
}

dgus_status_t dgus_desc_write_u16(dgus_ui_t *ui,
                                  uint16_t sp,
                                  uint16_t offset,
                                  uint16_t value)
{
    if ((ui == NULL) || (ui->dev == NULL) || (sp == DGUS_SP_NONE))
    {
        return DGUS_ERR_ARG;
    }

    return dgus_write_u16(ui->dev, (uint16_t)(sp + offset), value);
}

dgus_status_t dgus_desc_write_words(dgus_ui_t *ui,
                                    uint16_t sp,
                                    uint16_t offset,
                                    const uint16_t *words,
                                    uint8_t word_count)
{
    if ((ui == NULL) || (ui->dev == NULL) || (sp == DGUS_SP_NONE) ||
        (words == NULL) || (word_count == 0u))
    {
        return DGUS_ERR_ARG;
    }

    return dgus_write_words(ui->dev,
                            (uint16_t)(sp + offset),
                            words,
                            word_count);
}

dgus_status_t dgus_desc_write_point(dgus_ui_t *ui,
                                    uint16_t sp,
                                    uint16_t offset,
                                    dgus_point_t point)
{
    uint16_t words[2];

    words[0] = (uint16_t)point.x;
    words[1] = (uint16_t)point.y;

    return dgus_desc_write_words(ui, sp, offset, words, 2u);
}

dgus_status_t dgus_desc_write_rect(dgus_ui_t *ui,
                                   uint16_t sp,
                                   uint16_t offset,
                                   dgus_rect_t rect)
{
    uint16_t words[4];

    words[0] = (uint16_t)rect.x0;
    words[1] = (uint16_t)rect.y0;
    words[2] = (uint16_t)rect.x1;
    words[3] = (uint16_t)rect.y1;

    return dgus_desc_write_words(ui, sp, offset, words, 4u);
}

dgus_status_t dgus_obj_desc_write_bytes_at(dgus_ui_t *ui,
                                           const dgus_obj_t *obj,
                                           uint16_t sp,
                                           uint16_t offset,
                                           const uint8_t *data,
                                           uint16_t byte_len)
{
    dgus_status_t status;
    uint16_t address;

    if ((ui == NULL) || (ui->dev == NULL) || (obj == NULL) ||
        (sp == DGUS_SP_NONE) || (data == NULL) || (byte_len == 0u))
    {
        return DGUS_ERR_ARG;
    }

    address = (uint16_t)(sp + offset);
    status = dgus_write_bytes(ui->dev, address, data, byte_len);
    if (status == DGUS_OK)
    {
        dgus_obj_dispatch_param_changed(ui,
                                        obj,
                                        DGUS_WIDGET_PARAM_SP,
                                        address,
                                        offset,
                                        data,
                                        byte_len);
    }

    return status;
}

dgus_status_t dgus_obj_desc_write_u16_at(dgus_ui_t *ui,
                                         const dgus_obj_t *obj,
                                         uint16_t sp,
                                         uint16_t offset,
                                         uint16_t value)
{
    uint8_t data[2];

    dgus_put_u16_be(data, value);
    return dgus_obj_desc_write_bytes_at(ui,
                                        obj,
                                        sp,
                                        offset,
                                        data,
                                        sizeof(data));
}

dgus_status_t dgus_obj_desc_write_words_at(dgus_ui_t *ui,
                                           const dgus_obj_t *obj,
                                           uint16_t sp,
                                           uint16_t offset,
                                           const uint16_t *words,
                                           uint8_t word_count)
{
    uint8_t data[DGUS_TX_BUFFER_SIZE];
    uint16_t byte_len;
    uint8_t i;

    if ((words == NULL) || (word_count == 0u))
    {
        return DGUS_ERR_ARG;
    }

    byte_len = (uint16_t)(word_count * 2u);
    if (byte_len > (uint16_t)sizeof(data))
    {
        return DGUS_ERR_OVERFLOW;
    }

    for (i = 0u; i < word_count; i++)
    {
        dgus_put_u16_be(&data[(uint16_t)i * 2u], words[i]);
    }

    return dgus_obj_desc_write_bytes_at(ui,
                                        obj,
                                        sp,
                                        offset,
                                        data,
                                        byte_len);
}

dgus_status_t dgus_obj_desc_write_point_at(dgus_ui_t *ui,
                                           const dgus_obj_t *obj,
                                           uint16_t sp,
                                           uint16_t offset,
                                           dgus_point_t point)
{
    uint16_t words[2];

    words[0] = (uint16_t)point.x;
    words[1] = (uint16_t)point.y;

    return dgus_obj_desc_write_words_at(ui, obj, sp, offset, words, 2u);
}

dgus_status_t dgus_obj_desc_write_rect_at(dgus_ui_t *ui,
                                          const dgus_obj_t *obj,
                                          uint16_t sp,
                                          uint16_t offset,
                                          dgus_rect_t rect)
{
    uint16_t words[4];

    words[0] = (uint16_t)rect.x0;
    words[1] = (uint16_t)rect.y0;
    words[2] = (uint16_t)rect.x1;
    words[3] = (uint16_t)rect.y1;

    return dgus_obj_desc_write_words_at(ui, obj, sp, offset, words, 4u);
}

dgus_status_t dgus_ui_clear_touch_state(dgus_ui_t *ui)
{
    if ((ui == NULL) || (ui->dev == NULL))
    {
        return DGUS_ERR_ARG;
    }

    return dgus_write_u16(ui->dev, DGUS_SYS_ADDR_TOUCH_STATE, 0u);
}

