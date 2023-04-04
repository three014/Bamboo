#ifndef __OPTION_U32_H
#define __OPTION_U32_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct __Option_u32_Struct Option_u32;

Option_u32  *Option_u32_of(uint32_t item);
Option_u32  *Option_u32_empty();
void        Option_u32_delete(Option_u32 *self);
Option_u32  *Option_u32_clone(Option_u32 *self);
bool        Option_u32_is_some(Option_u32 *self);
bool        Option_u32_is_some_and(Option_u32 *self, bool (^predicate)(uint32_t item));
Option_u32  *Option_u32_take(Option_u32 *self);
bool        Option_u32_insert(Option_u32 *self, uint32_t item);
Option_u32  *Option_u32_replace(Option_u32 *self, uint32_t item);
uint32_t    Option_u32_get(Option_u32 *self);
uint32_t    Option_u32_get_or(Option_u32 *self, uint32_t default_value);
uint32_t    Option_u32_get_or_else(Option_u32 *self, uint32_t (^default_func)());
Option_u32  *Option_u32_map(Option_u32 *self, Option_u32 *(^map_func)(uint32_t item));
uint32_t    Option_u32_map_or_else(Option_u32 *self, uint32_t (^default_func)(), uint32_t (^map_func)(uint32_t item));
uint32_t    Option_u32_map_or(Option_u32 *self, uint32_t default_val, uint32_t (^map_func)(uint32_t item));
uint8_t     Option_u32_map_or_u8(Option_u32 *self, uint8_t default_val, uint8_t (^map_func)(uint32_t item));
uint16_t    Option_u32_map_or_u16(Option_u32 *self, uint16_t default_val, uint16_t (^map_func)(uint32_t item));
uint64_t    Option_u32_map_or_u64(Option_u32 *self, uint64_t default_val, uint64_t (^map_func)(uint32_t item));
int8_t      Option_u32_map_or_i8(Option_u32 *self, int8_t default_val, int8_t (^map_func)(uint32_t item));
int16_t     Option_u32_map_or_i16(Option_u32 *self, int16_t default_val, int16_t (^map_func)(uint32_t item)); 
int32_t     Option_u32_map_or_i32(Option_u32 *self, int32_t default_val, int32_t (^map_func)(uint32_t item));
int64_t     Option_u32_map_or_i64(Option_u32 *self, int64_t default_val, int64_t (^map_func)(uint32_t item));
size_t      Option_u32_map_or_size(Option_u32 *self, size_t default_val, size_t (^map_func)(uint32_t item));
float       Option_u32_map_or_f32(Option_u32 *self, float default_val, float (^map_func)(uint32_t item));
double      Option_u32_map_or_f64(Option_u32 *self, double default_val, double (^map_func)(uint32_t item));
uint8_t     Option_u32_map_or_else_u8(Option_u32 *self, uint8_t (^default_func)(), uint8_t (^map_func)(uint32_t item));
uint16_t    Option_u32_map_or_else_u16(Option_u32 *self, uint16_t (^default_func)(), uint16_t (^map_func)(uint32_t item));
uint64_t    Option_u32_map_or_else_u64(Option_u32 *self, uint64_t (^default_func)(), uint64_t (^map_func)(uint32_t item));
int8_t      Option_u32_map_or_else_i8(Option_u32 *self, int8_t (^default_func)(), int8_t (^map_func)(uint32_t item));
int16_t     Option_u32_map_or_else_i16(Option_u32 *self, int16_t (^default_func)(), int16_t (^map_func)(uint32_t item));
int32_t     Option_u32_map_or_else_i32(Option_u32 *self, int32_t (^default_func)(), int32_t (^map_func)(uint32_t item));
int64_t     Option_u32_map_or_else_i64(Option_u32 *self, int64_t (^default_func)(), int64_t (^map_func)(uint32_t item));
size_t      Option_u32_map_or_else_size(Option_u32 *self, size_t (^default_func)(), size_t (^map_func)(uint32_t item));
float       Option_u32_map_or_else_f32(Option_u32 *self, float (^default_func)(), float (^map_func)(uint32_t item));
double      Option_u32_map_or_else_f64(Option_u32 *self, double (^default_func)(), double (^map_func)(uint32_t item));

#endif
