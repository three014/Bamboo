#ifndef __OPTION_SIZE_H
#define __OPTION_SIZE_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>


typedef struct __Option_size_Struct Option_size;

Option_size          *Option_size_of(size_t item);
void            Option_size_delete(Option_size *self);
Option_size          *Option_size_clone(Option_size *self);
bool            Option_size_is_some(Option_size *self);
bool            Option_size_is_some_and(Option_size *self, bool (^predicate)(size_t item));
Option_size *Option_size_take(Option_size *self);
bool            Option_size_insert(Option_size *self, size_t item);
Option_size          *Option_size_replace(Option_size *self, size_t item);
size_t Option_size_get(Option_size *self);
size_t Option_size_get_or(Option_size *self, size_t default_value);
size_t Option_size_get_or_else(Option_size *self, size_t (^default_func)());
Option_size          *Option_size_map(Option_size *self, Option_size *(^map_func)(size_t item));
size_t Option_size_map_or_else(Option_size *self, size_t (^default_func)(), size_t (^map_func)(size_t item));
size_t Option_size_map_or(Option_size *self, size_t default_val, size_t (^map_func)(size_t item));
uint8_t         Option_size_map_or_u8(Option_size *self, uint8_t default_val, uint8_t (^map_func)(size_t item));
uint16_t        Option_size_map_or_u16(Option_size *self, uint16_t default_val, uint16_t (^map_func)(size_t item));
uint32_t        Option_size_map_or_u32(Option_size *self, uint32_t default_val, uint32_t (^map_func)(size_t item));
uint64_t        Option_size_map_or_u64(Option_size *self, uint64_t default_val, uint64_t (^map_func)(size_t item));
int8_t          Option_size_map_or_i8(Option_size *self, int8_t default_val, int8_t (^map_func)(size_t item));
int16_t         Option_size_map_or_i16(Option_size *self, int16_t default_val, int16_t (^map_func)(size_t item)); 
int32_t         Option_size_map_or_i32(Option_size *self, int32_t default_val, int32_t (^map_func)(size_t item));
int64_t         Option_size_map_or_i64(Option_size *self, int64_t default_val, int64_t (^map_func)(size_t item));
float           Option_size_map_or_f32(Option_size *self, float default_val, float (^map_func)(size_t item));
double          Option_size_map_or_f64(Option_size *self, double default_val, double (^map_func)(size_t item));
uint8_t         Option_size_map_or_else_u8(Option_size *self, uint8_t (^default_func)(), uint8_t (^map_func)(size_t item));
uint16_t        Option_size_map_or_else_u16(Option_size *self, uint16_t (^default_func)(), uint16_t (^map_func)(size_t item));
uint32_t        Option_size_map_or_else_u32(Option_size *self, uint32_t (^default_func)(), uint32_t (^map_func)(size_t item));
uint64_t        Option_size_map_or_else_u64(Option_size *self, uint64_t (^default_func)(), uint64_t (^map_func)(size_t item));
int8_t          Option_size_map_or_else_i8(Option_size *self, int8_t (^default_func)(), int8_t (^map_func)(size_t item));
int16_t         Option_size_map_or_else_i16(Option_size *self, int16_t (^default_func)(), int16_t (^map_func)(size_t item));
int32_t         Option_size_map_or_else_i32(Option_size *self, int32_t (^default_func)(), int32_t (^map_func)(size_t item));
int64_t         Option_size_map_or_else_i64(Option_size *self, int64_t (^default_func)(), int64_t (^map_func)(size_t item));
float           Option_size_map_or_else_f32(Option_size *self, float (^default_func)(), float (^map_func)(size_t item));
double          Option_size_map_or_else_f64(Option_size *self, double (^default_func)(), double (^map_func)(size_t item));

#endif
