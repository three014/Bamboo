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

Option *Option_as_ref(Option *self)
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

#define OPTION_MAP_FN(option, def, fn) \
    if (Option_is_some(option)) { \
        return (fn(Option_get(option))); \
    } else { \
        Option_delete(option); \
        return (default_val); \
    }

void        *Option_map_or      (Option *self, void     *default_val,   void       *(^map_func)(void *item)) { OPTION_MAP_FN(self, default_val, map_func) }
uint8_t     Option_map_or_u8    (Option *self, uint8_t  default_val,    uint8_t     (^map_func)(void *item)) { OPTION_MAP_FN(self, default_val, map_func) }
uint16_t    Option_map_or_u16   (Option *self, uint16_t default_val,    uint16_t    (^map_func)(void *item)) { OPTION_MAP_FN(self, default_val, map_func) }
uint32_t    Option_map_or_u32   (Option *self, uint32_t default_val,    uint32_t    (^map_func)(void *item)) { OPTION_MAP_FN(self, default_val, map_func) }
uint64_t    Option_map_or_u64   (Option *self, uint64_t default_val,    uint64_t    (^map_func)(void *item)) { OPTION_MAP_FN(self, default_val, map_func) }
int8_t      Option_map_or_i8    (Option *self, int8_t   default_val,    int8_t      (^map_func)(void *item)) { OPTION_MAP_FN(self, default_val, map_func) }
int16_t     Option_map_or_i16   (Option *self, int16_t  default_val,    int16_t     (^map_func)(void *item)) { OPTION_MAP_FN(self, default_val, map_func) }
int32_t     Option_map_or_i32   (Option *self, int32_t  default_val,    int32_t     (^map_func)(void *item)) { OPTION_MAP_FN(self, default_val, map_func) }
int64_t     Option_map_or_i64   (Option *self, int64_t  default_val,    int64_t     (^map_func)(void *item)) { OPTION_MAP_FN(self, default_val, map_func) }
size_t      Option_map_or_size  (Option *self, size_t   default_val,    size_t      (^map_func)(void *item)) { OPTION_MAP_FN(self, default_val, map_func) }
float       Option_map_or_f32   (Option *self, float    default_val,    float       (^map_func)(void *item)) { OPTION_MAP_FN(self, default_val, map_func) }
double      Option_map_or_f64   (Option *self, double   default_val,    double      (^map_func)(void *item)) { OPTION_MAP_FN(self, default_val, map_func) }





#define MAP_CLOSURE(new_type, curr_type, formal_arg_name, body) \
    ^ new_type (curr_type formal_arg_name) { body }


int bar() {
    return MAP_CLOSURE(int, float, c, { 
        return (int) c; 
    })('g');
}
