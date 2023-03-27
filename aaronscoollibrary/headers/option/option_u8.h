#ifndef __OPTION_U8_H
#define __OPTION_U8_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>


typedef struct __Option_u8_Struct Option_u8;

Option_u8          *Option_u8_of(uint8_t item);
void            Option_u8_delete(Option_u8 *self);
Option_u8          *Option_u8_clone(Option_u8 *self);
bool            Option_u8_is_some(Option_u8 *self);
bool            Option_u8_is_some_and(Option_u8 *self, bool (^predicate)(uint8_t item));
Option_u8 *Option_u8_take(Option_u8 *self);
bool            Option_u8_insert(Option_u8 *self, uint8_t item);
Option_u8          *Option_u8_replace(Option_u8 *self, uint8_t item);
uint8_t Option_u8_get(Option_u8 *self);
uint8_t Option_u8_get_or(Option_u8 *self, uint8_t default_value);
uint8_t Option_u8_get_or_else(Option_u8 *self, uint8_t (^default_func)());
Option_u8          *Option_u8_map(Option_u8 *self, Option_u8 *(^map_func)(uint8_t item));
uint8_t Option_u8_map_or_else(Option_u8 *self, uint8_t (^default_func)(), uint8_t (^map_func)(uint8_t item));
uint8_t Option_u8_map_or(Option_u8 *self, uint8_t default_val, uint8_t (^map_func)(uint8_t item));
uint16_t        Option_u8_map_or_u16(Option_u8 *self, uint16_t default_val, uint16_t (^map_func)(uint8_t item));
uint32_t        Option_u8_map_or_u32(Option_u8 *self, uint32_t default_val, uint32_t (^map_func)(uint8_t item));
uint64_t        Option_u8_map_or_u64(Option_u8 *self, uint64_t default_val, uint64_t (^map_func)(uint8_t item));
int8_t          Option_u8_map_or_i8(Option_u8 *self, int8_t default_val, int8_t (^map_func)(uint8_t item));
int16_t         Option_u8_map_or_i16(Option_u8 *self, int16_t default_val, int16_t (^map_func)(uint8_t item)); 
int32_t         Option_u8_map_or_i32(Option_u8 *self, int32_t default_val, int32_t (^map_func)(uint8_t item));
int64_t         Option_u8_map_or_i64(Option_u8 *self, int64_t default_val, int64_t (^map_func)(uint8_t item));
size_t          Option_u8_map_or_size(Option_u8 *self, size_t default_val, size_t (^map_func)(uint8_t item));
float           Option_u8_map_or_f32(Option_u8 *self, float default_val, float (^map_func)(uint8_t item));
double          Option_u8_map_or_f64(Option_u8 *self, double default_val, double (^map_func)(uint8_t item));
uint16_t        Option_u8_map_or_else_u16(Option_u8 *self, uint16_t (^default_func)(), uint16_t (^map_func)(uint8_t item));
uint32_t        Option_u8_map_or_else_u32(Option_u8 *self, uint32_t (^default_func)(), uint32_t (^map_func)(uint8_t item));
uint64_t        Option_u8_map_or_else_u64(Option_u8 *self, uint64_t (^default_func)(), uint64_t (^map_func)(uint8_t item));
int8_t          Option_u8_map_or_else_i8(Option_u8 *self, int8_t (^default_func)(), int8_t (^map_func)(uint8_t item));
int16_t         Option_u8_map_or_else_i16(Option_u8 *self, int16_t (^default_func)(), int16_t (^map_func)(uint8_t item));
int32_t         Option_u8_map_or_else_i32(Option_u8 *self, int32_t (^default_func)(), int32_t (^map_func)(uint8_t item));
int64_t         Option_u8_map_or_else_i64(Option_u8 *self, int64_t (^default_func)(), int64_t (^map_func)(uint8_t item));
size_t          Option_u8_map_or_else_size(Option_u8 *self, size_t (^default_func)(), size_t (^map_func)(uint8_t item));
float           Option_u8_map_or_else_f32(Option_u8 *self, float (^default_func)(), float (^map_func)(uint8_t item));
double          Option_u8_map_or_else_f64(Option_u8 *self, double (^default_func)(), double (^map_func)(uint8_t item));

#endif
