#ifndef __OBJECT_H
#define __OBJECT_H

#include <stddef.h>
#include <stdint.h>


typedef struct __Object_Wrapper_Struct {
    uint8_t hidden[9];
} ObjWrap;

ObjWrap Obj_from_str(const char *item);
ObjWrap Obj_from_u8(const uint8_t item);
ObjWrap Obj_from_u16(const uint16_t item);
ObjWrap Obj_from_u32(const uint32_t item);
ObjWrap Obj_from_u64(const uint64_t item);
ObjWrap Obj_from_i8(const int8_t item);
ObjWrap Obj_from_i16(const int16_t item);
ObjWrap Obj_from_i32(const int32_t item);
ObjWrap Obj_from_i64(const int64_t item);
ObjWrap Obj_from_float(const float item);
ObjWrap Obj_from_double(const double item);
ObjWrap Obj_from_size(const size_t item);
ObjWrap Obj_from_char(const char item);
ObjWrap Obj_from_ptr(const void *item);

char        *Obj_to_str(ObjWrap obj);
uint8_t     Obj_to_u8(ObjWrap obj);
uint16_t    Obj_to_u16(ObjWrap obj);
uint32_t    Obj_to_u32(ObjWrap obj);
uint64_t    Obj_to_u64(ObjWrap obj);
int8_t      Obj_to_i8(ObjWrap obj);
int16_t     Obj_to_i16(ObjWrap obj);
int32_t     Obj_to_i32(ObjWrap obj);
int64_t     Obj_to_i64(ObjWrap obj);
float       Obj_to_f32(ObjWrap obj);
double      Obj_to_f64(ObjWrap obj);
char        Obj_to_char(ObjWrap obj);
size_t      Obj_to_size(ObjWrap obj);
void        *Obj_to_ptr(ObjWrap obj);



#endif // !__OBJECT_H
