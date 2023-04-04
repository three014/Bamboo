#include "object_p.h"

#include <string.h>

#define IS_PTR 1
#define NOT_PTR 0

ObjectInternal ObjWrap_to_internal(ObjWrap item) {
    size_t real_item = 0;
    bool is_ptr = false;

    memcpy(&real_item, item.hidden, sizeof(size_t));
    is_ptr = item.hidden[8];

    return (ObjectInternal) {
        .is_ptr = is_ptr,
        .item = real_item
    };
}

void *ObjectInternal_to_view(const ObjectInternal *internal_item) {
    void *item = NULL;
    if (internal_item->is_ptr) {
        item = *(void **) &internal_item->item;
    }
    else {
        item = (void *) &internal_item->item;
    }
    return item;
}

#define TO_OBJ(the_item, is_ptr, type) { \
    ObjWrap obj; \
    memcpy(obj.hidden, & the_item, sizeof(type)); \
    obj.hidden[8] = (is_ptr); \
    return obj; \
}

ObjWrap Obj_str(const char *item) TO_OBJ(item, IS_PTR, char *);
ObjWrap Obj_u8(const uint8_t item) TO_OBJ(item, NOT_PTR, uint8_t);
ObjWrap Obj_u16(const uint16_t item) TO_OBJ(item, NOT_PTR, uint16_t);
ObjWrap Obj_u32(const uint32_t item) TO_OBJ(item, NOT_PTR, uint32_t);
ObjWrap Obj_u64(const uint64_t item) TO_OBJ(item, NOT_PTR, uint64_t);
ObjWrap Obj_i8(const int8_t item) TO_OBJ(item, NOT_PTR, int8_t);
ObjWrap Obj_i16(const int16_t item) TO_OBJ(item, NOT_PTR, int16_t);
ObjWrap Obj_i32(const int32_t item) TO_OBJ(item, NOT_PTR, int32_t);
ObjWrap Obj_i64(const int64_t item) TO_OBJ(item, NOT_PTR, int64_t);
ObjWrap Obj_float(const float item) TO_OBJ(item, NOT_PTR, float);
ObjWrap Obj_double(const double item) TO_OBJ(item, NOT_PTR, double);
ObjWrap Obj_size(const size_t item) TO_OBJ(item, NOT_PTR, size_t);
ObjWrap Obj_char(const char item) TO_OBJ(item, NOT_PTR, char);
ObjWrap Obj_ptr(const void *item) TO_OBJ(item, IS_PTR, void *);

