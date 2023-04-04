#ifndef __OPTION_F32_H
#define __OPTION_F32_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>


typedef struct __Option_f32_Struct Option_f32;

Option_f32      *Option_f32_of(float item);
Option_f32      *Option_f32_empty();
void            Option_f32_delete(Option_f32 *self);
Option_f32      *Option_f32_clone(Option_f32 *self);
bool            Option_f32_is_some(Option_f32 *self);
bool            Option_f32_is_some_and(Option_f32 *self, bool (^predicate)(float item));
Option_f32      *Option_f32_take(Option_f32 *self);
bool            Option_f32_insert(Option_f32 *self, float item);
Option_f32      *Option_f32_replace(Option_f32 *self, float item);
float           Option_f32_get(Option_f32 *self);
float           Option_f32_get_or(Option_f32 *self, float default_value);
float           Option_f32_get_or_else(Option_f32 *self, float (^default_func)());
Option_f32      *Option_f32_map(Option_f32 *self, Option_f32 *(^map_func)(float item));
float           Option_f32_map_or_else(Option_f32 *self, float (^default_func)(), float (^map_func)(float item));
float           Option_f32_map_or(Option_f32 *self, float default_val, float (^map_func)(float item));
uint8_t         Option_f32_map_or_u8(Option_f32 *self, uint8_t default_val, uint8_t (^map_func)(float item));
uint16_t        Option_f32_map_or_u16(Option_f32 *self, uint16_t default_val, uint16_t (^map_func)(float item));
uint32_t        Option_f32_map_or_u32(Option_f32 *self, uint32_t default_val, uint32_t (^map_func)(float item));
uint64_t        Option_f32_map_or_u64(Option_f32 *self, uint64_t default_val, uint64_t (^map_func)(float item));
int8_t          Option_f32_map_or_i8(Option_f32 *self, int8_t default_val, int8_t (^map_func)(float item));
int16_t         Option_f32_map_or_i16(Option_f32 *self, int16_t default_val, int16_t (^map_func)(float item)); 
int32_t         Option_f32_map_or_i32(Option_f32 *self, int32_t default_val, int32_t (^map_func)(float item));
int64_t         Option_f32_map_or_i64(Option_f32 *self, int64_t default_val, int64_t (^map_func)(float item));
size_t          Option_f32_map_or_size(Option_f32 *self, size_t default_val, size_t (^map_func)(float item));
double          Option_f32_map_or_f64(Option_f32 *self, double default_val, double (^map_func)(float item));
uint8_t         Option_f32_map_or_else_u8(Option_f32 *self, uint8_t (^default_func)(), uint8_t (^map_func)(float item));
uint16_t        Option_f32_map_or_else_u16(Option_f32 *self, uint16_t (^default_func)(), uint16_t (^map_func)(float item));
uint32_t        Option_f32_map_or_else_u32(Option_f32 *self, uint32_t (^default_func)(), uint32_t (^map_func)(float item));
uint64_t        Option_f32_map_or_else_u64(Option_f32 *self, uint64_t (^default_func)(), uint64_t (^map_func)(float item));
int8_t          Option_f32_map_or_else_i8(Option_f32 *self, int8_t (^default_func)(), int8_t (^map_func)(float item));
int16_t         Option_f32_map_or_else_i16(Option_f32 *self, int16_t (^default_func)(), int16_t (^map_func)(float item));
int32_t         Option_f32_map_or_else_i32(Option_f32 *self, int32_t (^default_func)(), int32_t (^map_func)(float item));
int64_t         Option_f32_map_or_else_i64(Option_f32 *self, int64_t (^default_func)(), int64_t (^map_func)(float item));
size_t          Option_f32_map_or_else_size(Option_f32 *self, size_t (^default_func)(), size_t (^map_func)(float item));
double          Option_f32_map_or_else_f64(Option_f32 *self, double (^default_func)(), double (^map_func)(float item));

#endif
