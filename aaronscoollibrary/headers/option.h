#ifndef __OPTION_H
#define __OPTION_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct __Option_Struct Option;
typedef struct __Option_i32_Struct Option_i32;
typedef struct __Option_u32_Struct Option_u32;



Option          *Option_of_ref(void **item);
Option          *Option_of(void *item);
void            Option_delete(Option *self);
Option          *Option_clone(Option *self);
bool            Option_is_some(Option *self);
bool            Option_is_some_and(Option *self, bool (^predicate)(void *item));
void            *Option_take(Option *self);
bool            Option_insert(Option *self, void *item);
Option          *Option_replace(Option *self, void *item);
void            *Option_get(Option *self);
void            *Option_get_or(Option *self, void *default_value);
void            *Option_get_or_else(Option *self, void *(^default_func)());
Option          *Option_map(Option *self, Option *(^map_func)(void *item));
void            *Option_map_or_else(Option *self, void *(^default_func)(), void *(^map_func)(void *item));
void            *Option_map_or(Option *self, void *default_val, void *(^map_func)(void *item));
uint8_t         Option_map_or_u8(Option *self, uint8_t default_val, uint8_t (^map_func)(void *item));
uint16_t        Option_map_or_u16(Option *self, uint16_t default_val, uint16_t (^map_func)(void *item));
uint32_t        Option_map_or_u32(Option *self, uint32_t default_val, uint32_t (^map_func)(void *item));
uint64_t        Option_map_or_u64(Option *self, uint64_t default_val, uint64_t (^map_func)(void *item));
int8_t          Option_map_or_i8(Option *self, int8_t default_val, int8_t (^map_func)(void *item));
int16_t         Option_map_or_i16(Option *self, int16_t default_val, int16_t (^map_func)(void *item)); 
int32_t         Option_map_or_i32(Option *self, int32_t default_val, int32_t (^map_func)(void *item));
int64_t         Option_map_or_i64(Option *self, int64_t default_val, int64_t (^map_func)(void *item));
size_t          Option_map_or_size(Option *self, size_t default_val, size_t (^map_func)(void *item));
float           Option_map_or_f32(Option *self, float default_val, float (^map_func)(void *item));
double          Option_map_or_f64(Option *self, double default_val, double (^map_func)(void *item));
uint8_t         Option_map_or_else_u8(Option *self, uint8_t (^default_func)(), uint8_t (^map_func)(void *item));
uint16_t        Option_map_or_else_u16(Option *self, uint16_t (^default_func)(), uint16_t (^map_func)(void *item));
uint32_t        Option_map_or_else_u32(Option *self, uint32_t (^default_func)(), uint32_t (^map_func)(void *item));
uint64_t        Option_map_or_else_u64(Option *self, uint64_t (^default_func)(), uint64_t (^map_func)(void *item));
int8_t          Option_map_or_else_i8(Option *self, int8_t (^default_func)(), int8_t (^map_func)(void *item));
int16_t         Option_map_or_else_i16(Option *self, int16_t (^default_func)(), int16_t (^map_func)(void *item));
int32_t         Option_map_or_else_i32(Option *self, int32_t (^default_func)(), int32_t (^map_func)(void *item));
int64_t         Option_map_or_else_i64(Option *self, int64_t (^default_func)(), int64_t (^map_func)(void *item));
size_t          Option_map_or_else_size(Option *self, size_t (^default_func)(), size_t (^map_func)(void *item));
float           Option_map_or_else_f32(Option *self, float (^default_func)(), float (^map_func)(void *item));
double          Option_map_or_else_f64(Option *self, double (^default_func)(), double (^map_func)(void *item));

/* i32 Option */
Option_i32      *Option_i32_of(int32_t item);
void            Option_i32_delete(Option_i32 *self);
Option_i32      *Option_i32_clone(Option_i32 *self);
bool            Option_i32_is_some(Option_i32 *self);
bool            Option_i32_is_some_and(Option_i32 *self, bool (^predicate)(int32_t item));
Option_i32      *Option_i32_take(Option_i32 *self);
bool            Option_i32_insert(Option_i32 *self, int32_t item);
Option_i32      *Option_i32_replace(Option_i32 *self, int32_t item);
int32_t         Option_i32_get(Option_i32 *self);
int32_t         Option_i32_get_or(Option_i32 *self, int32_t default_value);
int32_t         Option_i32_get_or_else(Option_i32 *self, int32_t (^default_func)());

/* Not implemented yet */
Option_i32      *Option_i32_map(Option_i32 *self, Option_i32 *(^map_func)(int32_t item));
Option          *Option_i32_map_generic(Option_i32 *self, Option *(^map_func)(int32_t item));
void            *Option_i32_map_or_else_generic(Option_i32 *self, void *(^default_func)(), void *(^map_func)(int32_t item));
void            *Option_i32_map_or_generic(Option *self, void *default_val, void *(^map_func)(int32_t item));



#endif
