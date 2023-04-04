#ifndef __OPTION_F64_H
#define __OPTION_F64_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>


typedef struct __Option_f64_Struct Option_f64;

Option_f64  *Option_f64_of(double item);
Option_f64  *Option_f64_empty();
void        Option_f64_delete(Option_f64 *self);
Option_f64  *Option_f64_clone(Option_f64 *self);
bool        Option_f64_is_some(Option_f64 *self);
bool        Option_f64_is_some_and(Option_f64 *self, bool (^predicate)(double item));
Option_f64  *Option_f64_take(Option_f64 *self);
bool        Option_f64_insert(Option_f64 *self, double item);
Option_f64  *Option_f64_replace(Option_f64 *self, double item);
double      Option_f64_get(Option_f64 *self);
double      Option_f64_get_or(Option_f64 *self, double default_value);
double      Option_f64_get_or_else(Option_f64 *self, double (^default_func)());
Option_f64  *Option_f64_map(Option_f64 *self, Option_f64 *(^map_func)(double item));
double      Option_f64_map_or_else(Option_f64 *self, double (^default_func)(), double (^map_func)(double item));
double      Option_f64_map_or(Option_f64 *self, double default_val, double (^map_func)(double item));
uint8_t     Option_f64_map_or_u8(Option_f64 *self, uint8_t default_val, uint8_t (^map_func)(double item));
uint16_t    Option_f64_map_or_u16(Option_f64 *self, uint16_t default_val, uint16_t (^map_func)(double item));
uint32_t    Option_f64_map_or_u32(Option_f64 *self, uint32_t default_val, uint32_t (^map_func)(double item));
uint64_t    Option_f64_map_or_u64(Option_f64 *self, uint64_t default_val, uint64_t (^map_func)(double item));
int8_t      Option_f64_map_or_i8(Option_f64 *self, int8_t default_val, int8_t (^map_func)(double item));
int16_t     Option_f64_map_or_i16(Option_f64 *self, int16_t default_val, int16_t (^map_func)(double item)); 
int32_t     Option_f64_map_or_i32(Option_f64 *self, int32_t default_val, int32_t (^map_func)(double item));
int64_t     Option_f64_map_or_i64(Option_f64 *self, int64_t default_val, int64_t (^map_func)(double item));
size_t      Option_f64_map_or_size(Option_f64 *self, size_t default_val, size_t (^map_func)(double item));
float       Option_f64_map_or_f32(Option_f64 *self, float default_val, float (^map_func)(double item));
uint8_t     Option_f64_map_or_else_u8(Option_f64 *self, uint8_t (^default_func)(), uint8_t (^map_func)(double item));
uint16_t    Option_f64_map_or_else_u16(Option_f64 *self, uint16_t (^default_func)(), uint16_t (^map_func)(double item));
uint32_t    Option_f64_map_or_else_u32(Option_f64 *self, uint32_t (^default_func)(), uint32_t (^map_func)(double item));
uint64_t    Option_f64_map_or_else_u64(Option_f64 *self, uint64_t (^default_func)(), uint64_t (^map_func)(double item));
int8_t      Option_f64_map_or_else_i8(Option_f64 *self, int8_t (^default_func)(), int8_t (^map_func)(double item));
int16_t     Option_f64_map_or_else_i16(Option_f64 *self, int16_t (^default_func)(), int16_t (^map_func)(double item));
int32_t     Option_f64_map_or_else_i32(Option_f64 *self, int32_t (^default_func)(), int32_t (^map_func)(double item));
int64_t     Option_f64_map_or_else_i64(Option_f64 *self, int64_t (^default_func)(), int64_t (^map_func)(double item));
size_t      Option_f64_map_or_else_size(Option_f64 *self, size_t (^default_func)(), size_t (^map_func)(double item));
float       Option_f64_map_or_else_f32(Option_f64 *self, float (^default_func)(), float (^map_func)(double item));

#endif
