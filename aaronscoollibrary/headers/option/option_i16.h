#ifndef __OPTION_I16_H
#define __OPTION_I16_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct __Option_i16_Struct Option_i16;

Option_i16          *Option_i16_of(int16_t item);
void            Option_i16_delete(Option_i16 *self);
Option_i16          *Option_i16_clone(Option_i16 *self);
bool            Option_i16_is_some(Option_i16 *self);
bool            Option_i16_is_some_and(Option_i16 *self, bool (^predicate)(int16_t item));
Option_i16 *Option_i16_take(Option_i16 *self);
bool            Option_i16_insert(Option_i16 *self, int16_t item);
Option_i16          *Option_i16_replace(Option_i16 *self, int16_t item);
int16_t Option_i16_get(Option_i16 *self);
int16_t Option_i16_get_or(Option_i16 *self, int16_t default_value);
int16_t Option_i16_get_or_else(Option_i16 *self, int16_t (^default_func)());
Option_i16          *Option_i16_map(Option_i16 *self, Option_i16 *(^map_func)(int16_t item));
int16_t Option_i16_map_or_else(Option_i16 *self, int16_t (^default_func)(), int16_t (^map_func)(int16_t item));
int16_t Option_i16_map_or(Option_i16 *self, int16_t default_val, int16_t (^map_func)(int16_t item));
uint8_t         Option_i16_map_or_u8(Option_i16 *self, uint8_t default_val, uint8_t (^map_func)(int16_t item));
uint16_t        Option_i16_map_or_u16(Option_i16 *self, uint16_t default_val, uint16_t (^map_func)(int16_t item));
uint32_t        Option_i16_map_or_u32(Option_i16 *self, uint32_t default_val, uint32_t (^map_func)(int16_t item));
uint64_t        Option_i16_map_or_u64(Option_i16 *self, uint64_t default_val, uint64_t (^map_func)(int16_t item));
int8_t          Option_i16_map_or_i8(Option_i16 *self, int8_t default_val, int8_t (^map_func)(int16_t item));
int32_t         Option_i16_map_or_i32(Option_i16 *self, int32_t default_val, int32_t (^map_func)(int16_t item));
int64_t         Option_i16_map_or_i64(Option_i16 *self, int64_t default_val, int64_t (^map_func)(int16_t item));
size_t          Option_i16_map_or_size(Option_i16 *self, size_t default_val, size_t (^map_func)(int16_t item));
float           Option_i16_map_or_f32(Option_i16 *self, float default_val, float (^map_func)(int16_t item));
double          Option_i16_map_or_f64(Option_i16 *self, double default_val, double (^map_func)(int16_t item));
uint8_t         Option_i16_map_or_else_u8(Option_i16 *self, uint8_t (^default_func)(), uint8_t (^map_func)(int16_t item));
uint16_t        Option_i16_map_or_else_u16(Option_i16 *self, uint16_t (^default_func)(), uint16_t (^map_func)(int16_t item));
uint32_t        Option_i16_map_or_else_u32(Option_i16 *self, uint32_t (^default_func)(), uint32_t (^map_func)(int16_t item));
uint64_t        Option_i16_map_or_else_u64(Option_i16 *self, uint64_t (^default_func)(), uint64_t (^map_func)(int16_t item));
int8_t          Option_i16_map_or_else_i8(Option_i16 *self, int8_t (^default_func)(), int8_t (^map_func)(int16_t item));
int32_t         Option_i16_map_or_else_i32(Option_i16 *self, int32_t (^default_func)(), int32_t (^map_func)(int16_t item));
int64_t         Option_i16_map_or_else_i64(Option_i16 *self, int64_t (^default_func)(), int64_t (^map_func)(int16_t item));
size_t          Option_i16_map_or_else_size(Option_i16 *self, size_t (^default_func)(), size_t (^map_func)(int16_t item));
float           Option_i16_map_or_else_f32(Option_i16 *self, float (^default_func)(), float (^map_func)(int16_t item));
double          Option_i16_map_or_else_f64(Option_i16 *self, double (^default_func)(), double (^map_func)(int16_t item));

#endif
