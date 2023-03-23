#include "option.h"
#include <stdio.h>

struct __Option_Struct
{
    bool some;
    void *item;
};

/// @brief Creates an Option object from a NULL or valid reference.
/// @param item Address of the pointer to the object.
/// @return Option object possibly containing a valid pointer to an object.
Option *Option_of_ref(void **item)
{
    Option *ret = malloc(sizeof *ret);
    if (item == NULL || *item == NULL)
    {
        // printf("Option: item not wrapped successfully\n");
        ret->item = NULL;
        ret->some = false;
    }
    else 
    {
        //printf("option item: %p\n", item);
        //printf("option deref: %llu\n", *(void **) item);
        // printf("Option: item wrapped successfully\n");
        ret->item = *item;
        ret->some = true;
    }
    return ret;
}

Option *Option_of(void *item)
{
    Option *ret = malloc(sizeof *ret);
    if (item == NULL)
    {
        ret->item = NULL;
        ret->some = false;
    }
    else 
    {
        ret->item = item;
        ret->some = true;
    }
    return ret;
}

void Option_delete(Option *self)
{
    self->item = NULL;
    free(self);
}

Option *Option_clone(Option *self)
{
    return Option_of(self->item);
}

bool Option_is_some(Option *self)
{
    if (self == NULL)
    {
        return false;
    }
    return self->some;
}

bool Option_is_some_and(Option *self, bool (^predicate)(void *item))
{
    if (self == NULL 
            || Option_is_some(self) == false 
            || predicate == NULL)
    {
        return false;
    }
    return predicate(self->item);
}

void *Option_take(Option *self)
{
    void *ret = self->item;
    self->item = NULL;
    self->some = false;
    return ret;
}

bool Option_insert(Option *self, void *item)
{
    if (Option_is_some(self) || item == NULL)
    {
        return false;
    } 
    else 
    {
        self->item = item;
        self->some = true;
    }
    return true;
}

Option *Option_replace(Option *self, void *item)
{
    Option *ret = Option_of(self->item);
    self->item = item;
    if (item == NULL)
    {
        self->some = false;
    }
    else
    {
        self->some = true;
    }
    return ret;
}

void *Option_get(Option *self)
{
    void *ret = self->item;
    self->item = NULL;
    Option_delete(self);
    return ret;
}

void *Option_get_or_else(Option *self, void *(^default_func)())
{
    if (Option_is_some(self))
    {
        return Option_get(self);
    }
    else if (default_func == NULL)
    {
        Option_delete(self);
        return NULL;
    }
    else 
    {
        Option_delete(self);
        return default_func();
    }
}

void *Option_get_or(Option *self, void *default_value)
{
    if (Option_is_some(self))
    {
        return Option_get(self);
    }
    else 
    {
        Option_delete(self);
        return default_value;
    }
}

Option *Option_map(Option *self, Option *(^map_func)(void *item))
{
    if (Option_is_some(self))
    {
        return map_func(Option_get(self));
    }
    else
    {
        return self;
    }
}

void *Option_map_or_else(Option *self, void *(^default_func)(), void *(^map_func)(void *item)) 
{
    if (Option_is_some(self))
    {
        return map_func(Option_get(self));
    }
    else 
    {
        Option_delete(self);
        return default_func();
    }
}

#define OPTION_MAP_OR_FN(option, def, fn) \
    if (Option_is_some(option)) { \
        return (fn(Option_get(option))); \
    } else { \
        Option_delete(option); \
        return (default_val); \
    }
#define OPTION_MAP_OR_ELSE_FN(option, def_fn, map_fn) \
    if (Option_is_some(option)) { \
        return (map_fn(Option_get(option))); \
    } else { \
        return (def_fn()); \
    }


void *Option_map_or(Option *self, void *default_val, void *(^map_func)(void *item)) { OPTION_MAP_OR_FN(self, default_val, map_func) }

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


#define OPTION_MAP_OR_FACTORY(type) \
type Option_map_or_##type (Option *self, type default_val, type (^map_fn)(void *item)) { OPTION_MAP_OR_FN(self, default_val, map_fn) }

#define OPTION_MAP_OR_ELSE_FACTORY(type) \
type Option_map_or_else_##type (Option *self, type (^default_fn)(), type (^map_fn)(void *item)) { OPTION_MAP_OR_ELSE_FN(self, default_fn, map_fn) }

OPTION_MAP_OR_FACTORY(i8);
OPTION_MAP_OR_FACTORY(i16);
OPTION_MAP_OR_FACTORY(i32);
OPTION_MAP_OR_FACTORY(i64);
OPTION_MAP_OR_FACTORY(u8);
OPTION_MAP_OR_FACTORY(u16);
OPTION_MAP_OR_FACTORY(u32);
OPTION_MAP_OR_FACTORY(u64);
OPTION_MAP_OR_FACTORY(f32);
OPTION_MAP_OR_FACTORY(f64);
OPTION_MAP_OR_FACTORY(size);

OPTION_MAP_OR_ELSE_FACTORY(i8);
OPTION_MAP_OR_ELSE_FACTORY(i16);
OPTION_MAP_OR_ELSE_FACTORY(i32);
OPTION_MAP_OR_ELSE_FACTORY(i64);
OPTION_MAP_OR_ELSE_FACTORY(u8);
OPTION_MAP_OR_ELSE_FACTORY(u16);
OPTION_MAP_OR_ELSE_FACTORY(u32);
OPTION_MAP_OR_ELSE_FACTORY(u64);
OPTION_MAP_OR_ELSE_FACTORY(f32);
OPTION_MAP_OR_ELSE_FACTORY(f64);
OPTION_MAP_OR_ELSE_FACTORY(size);

