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

ObjWrap ObjectInternal_to_wrap(ObjectInternal internal_item) {
    ObjWrap obj;
    memcpy(obj.hidden, &internal_item.item, sizeof(size_t));
    obj.hidden[8] = internal_item.is_ptr;
    return obj;
}

#define TO_OBJ(the_item, is_ptr, type) { \
    ObjWrap obj; \
    memcpy(obj.hidden, & the_item, sizeof(type)); \
    obj.hidden[8] = (is_ptr); \
    return obj; \
}

ObjWrap Obj_from_str(const char *item) TO_OBJ(item, IS_PTR, char *)
ObjWrap Obj_from_u8(const uint8_t item) TO_OBJ(item, NOT_PTR, uint8_t)
ObjWrap Obj_from_u16(const uint16_t item) TO_OBJ(item, NOT_PTR, uint16_t)
ObjWrap Obj_from_u32(const uint32_t item) TO_OBJ(item, NOT_PTR, uint32_t)
ObjWrap Obj_from_u64(const uint64_t item) TO_OBJ(item, NOT_PTR, uint64_t)
ObjWrap Obj_from_i8(const int8_t item) TO_OBJ(item, NOT_PTR, int8_t)
ObjWrap Obj_from_i16(const int16_t item) TO_OBJ(item, NOT_PTR, int16_t)
ObjWrap Obj_from_i32(const int32_t item) TO_OBJ(item, NOT_PTR, int32_t)
ObjWrap Obj_from_i64(const int64_t item) TO_OBJ(item, NOT_PTR, int64_t)
ObjWrap Obj_from_f32(const float item) TO_OBJ(item, NOT_PTR, float)
ObjWrap Obj_from_f64(const double item) TO_OBJ(item, NOT_PTR, double)
ObjWrap Obj_from_size(const size_t item) TO_OBJ(item, NOT_PTR, size_t)
ObjWrap Obj_from_char(const char item) TO_OBJ(item, NOT_PTR, char)
ObjWrap Obj_from_ptr(const void *item) TO_OBJ(item, IS_PTR, void *)

#define TO_TYPE(the_obj, type) { \
    type ret = *(type *) &the_obj.hidden; \
    return ret; \
}

char *Obj_to_str(ObjWrap obj) TO_TYPE(obj, char *)
uint8_t Obj_to_u8(ObjWrap obj) TO_TYPE(obj, uint8_t)
uint16_t Obj_to_u16(ObjWrap obj) TO_TYPE(obj, uint16_t)
uint32_t Obj_to_u32(ObjWrap obj) TO_TYPE(obj, uint32_t)
uint64_t Obj_to_u64(ObjWrap obj) TO_TYPE(obj, uint64_t)
int8_t Obj_to_i8(ObjWrap obj) TO_TYPE(obj, int8_t)
int16_t Obj_to_i16(ObjWrap obj) TO_TYPE(obj, int16_t)
int32_t Obj_to_i32(ObjWrap obj) TO_TYPE(obj, int32_t)
int64_t Obj_to_i64(ObjWrap obj) TO_TYPE(obj, int64_t)
float Obj_to_f32(ObjWrap obj) TO_TYPE(obj, float)
double Obj_to_f64(ObjWrap obj) TO_TYPE(obj, double)
size_t Obj_to_size(ObjWrap obj) TO_TYPE(obj, size_t)
char Obj_to_char(ObjWrap obj) TO_TYPE(obj, char)
void *Obj_to_ptr(ObjWrap obj) TO_TYPE(obj, void *)
