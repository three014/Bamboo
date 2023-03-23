#ifndef __OPTION_TEMPLATE_H
#define __OPTION_TEMPLATE_H

#include <stddef.h>
#include <stdint.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef size_t size;
typedef float f32;
typedef double f64;

#define OPTION_TYPE_FACTORY(type) \
struct __Option_##type##_Struct \
{ \
    type item; \
    bool some; \
};


#define OPTION_TYPE_FUNCTION_FACTORY(type) \
Option_##type *Option_##type##_of(type item) \
{ \
    Option_##type *ret = malloc(sizeof *ret); \
    ret->item = item; \
    ret->some = true; \
    return ret; \
} \
Option_##type *Option_##type##_empty() \
{ \
    Option_##type *ret = malloc(sizeof *ret); \
    ret->some = false; \
    return ret; \
} \
void Option_##type##_delete(Option_##type *self) \
{ \
    free(self); \
} \
Option_##type *Option_##type##_clone(Option_##type *self) \
{ \
    return Option_##type##_of(self->item); \
} \
bool Option_##type##_is_some(Option_##type *self) \
{ \
    if (self == NULL) \
    { \
        return false; \
    } \
    return self->some; \
} \
bool Option_##type##_is_some_and(Option_##type *self, bool (^predicate)(type item)) \
{ \
    if (!Option_##type##_is_some(self) \
            || predicate == NULL) \
    { \
        return false; \
    } \
    return predicate(self->item); \
} \
Option_##type *Option_##type##_take(Option_##type *self) \
{ \
    if (!Option_##type##_is_some(self)) \
    { \
        return Option_##type##_empty(); \
    } \
    self->item = (type) 0; \
    self->some = false; \
    return Option_##type##_of(self->item); \
} \
bool Option_##type##_insert(Option_##type *self, type item) \
{ \
    if (Option_##type##_is_some(self)) \
    { \
        return false; \
    } \
    else \
    { \
        self->item = item; \
        self->some = true; \
    } \
    return true; \
} \
Option_##type *Option_##type##_replace(Option_##type *self, type item) \
{ \
    if (Option_##type##_is_some(self)) \
    { \
        Option_##type *ret = Option_##type##_of(self->item); \
        self->item = item; \
        return ret; \
    } \
    else \
    { \
        Option_##type *ret = Option_##type##_empty(); \
        self->item = item; \
        return ret; \
    } \
} \
type Option_##type##_get(Option_##type *self) \
{ \
    if (!Option_##type##_is_some(self)) return (type) 0; \
    type ret = self->item; \
    self->item = (type) 0; \
    Option_##type##_delete(self); \
    return ret; \
} \
type Option_##type##_get_or_else(Option_##type *self, type (^default_func)()) \
{ \
    if (Option_##type##_is_some(self)) \
    { \
        return Option_##type##_get(self); \
    } \
    else if (default_func == NULL) \
    { \
        Option_##type##_delete(self); \
        return (type) 0; \
    } \
    else \
    { \
        Option_##type##_delete(self); \
        return default_func(); \
    } \
} \
type Option_##type##_get_or(Option_##type *self, type default_value) \
{ \
    if (Option_##type##_is_some(self)) \
    { \
        return Option_##type##_get(self); \
    } \
    else \
    { \
        Option_##type##_delete(self); \
        return default_value; \
    } \
} \
Option_##type *Option_##type##_map(Option_##type *self, Option_##type *(^map_func)(type item)) \
{ \
    if (Option_##type##_is_some(self)) \
    { \
        return (map_func(Option_##type##_get(self))); \
    } \
    else \
    { \
        return self; \
    } \
} \
type Option_##type##_map_or(Option_##type *self, type default_val, type (^map_func)(type item)) \
{ \
    if (Option_##type##_is_some(self)) \
    { \
        return (map_func(Option_##type##_get(self))); \
    } \
    else \
    { \
        return default_val; \
    } \
} \

#endif
