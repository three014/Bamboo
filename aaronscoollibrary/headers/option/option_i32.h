#ifndef __OPTION_I32_H
#define __OPTION_I32_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct __Option_i32_Struct Option_i32;

Option_i32  *Option_i32_of(int32_t item);
Option_i32  *Option_i32_empty();
void        Option_i32_delete(Option_i32 *self);
Option_i32  *Option_i32_clone(Option_i32 *self);
bool        Option_i32_is_some(Option_i32 *self);
bool        Option_i32_is_some_and(Option_i32 *self, bool (^predicate)(int32_t item));
Option_i32  *Option_i32_take(Option_i32 *self);
bool        Option_i32_insert(Option_i32 *self, int32_t item);
Option_i32  *Option_i32_replace(Option_i32 *self, int32_t item);
int32_t     Option_i32_get(Option_i32 *self);
int32_t     Option_i32_get_or(Option_i32 *self, int32_t default_value);
int32_t     Option_i32_get_or_else(Option_i32 *self, int32_t (^default_func)());
Option_i32  *Option_i32_map(Option_i32 *self, Option_i32 *(^map_func)(int32_t item));
int32_t     Option_i32_map_or_else(Option_i32 *self, int32_t (^default_func)(), int32_t (^map_func)(int32_t item));
int32_t     Option_i32_map_or(Option_i32 *self, int32_t default_val, int32_t (^map_func)(int32_t item));
uint8_t     Option_i32_map_or_u8(Option_i32 *self, uint8_t default_val, uint8_t (^map_func)(int32_t item));
uint16_t    Option_i32_map_or_u16(Option_i32 *self, uint16_t default_val, uint16_t (^map_func)(int32_t item));
uint32_t    Option_i32_map_or_u32(Option_i32 *self, uint32_t default_val, uint32_t (^map_func)(int32_t item));
uint64_t    Option_i32_map_or_u64(Option_i32 *self, uint64_t default_val, uint64_t (^map_func)(int32_t item));
int8_t      Option_i32_map_or_i8(Option_i32 *self, int8_t default_val, int8_t (^map_func)(int32_t item));
int16_t     Option_i32_map_or_i16(Option_i32 *self, int16_t default_val, int16_t (^map_func)(int32_t item)); 
int64_t     Option_i32_map_or_i64(Option_i32 *self, int64_t default_val, int64_t (^map_func)(int32_t item));
size_t      Option_i32_map_or_size(Option_i32 *self, size_t default_val, size_t (^map_func)(int32_t item));
float       Option_i32_map_or_f32(Option_i32 *self, float default_val, float (^map_func)(int32_t item));
double      Option_i32_map_or_f64(Option_i32 *self, double default_val, double (^map_func)(int32_t item));
uint8_t     Option_i32_map_or_else_u8(Option_i32 *self, uint8_t (^default_func)(), uint8_t (^map_func)(int32_t item));
uint16_t    Option_i32_map_or_else_u16(Option_i32 *self, uint16_t (^default_func)(), uint16_t (^map_func)(int32_t item));
uint32_t    Option_i32_map_or_else_u32(Option_i32 *self, uint32_t (^default_func)(), uint32_t (^map_func)(int32_t item));
uint64_t    Option_i32_map_or_else_u64(Option_i32 *self, uint64_t (^default_func)(), uint64_t (^map_func)(int32_t item));
int8_t      Option_i32_map_or_else_i8(Option_i32 *self, int8_t (^default_func)(), int8_t (^map_func)(int32_t item));
int16_t     Option_i32_map_or_else_i16(Option_i32 *self, int16_t (^default_func)(), int16_t (^map_func)(int32_t item));
int64_t     Option_i32_map_or_else_i64(Option_i32 *self, int64_t (^default_func)(), int64_t (^map_func)(int32_t item));
size_t      Option_i32_map_or_else_size(Option_i32 *self, size_t (^default_func)(), size_t (^map_func)(int32_t item));
float       Option_i32_map_or_else_f32(Option_i32 *self, float (^default_func)(), float (^map_func)(int32_t item));
double      Option_i32_map_or_else_f64(Option_i32 *self, double (^default_func)(), double (^map_func)(int32_t item));

#endif
