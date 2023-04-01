#include "vector/vector.h"
#include "interface/iterator.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


////////////////////////////////////////////////////
//                                                //
//  Vector: A Self-Resizing Array, written in C   // 
//                                                //
//  Aaron Perez                                   //
//  March 2023                                    //
//                                                //
//  (Concept is not original, and neither is      //
//  implementation, probably)                     //
//                                                //
////////////////////////////////////////////////////


// Stuff for the generic vector //


#define DEFAULT_CAPACITY 8
#define RESIZE_CONSTANT 2

struct __Vector_Struct
{
    size_t capacity;
    size_t length;
    void **arr;
};

typedef struct __Vector_Iterator
{
    Vec *vec;
    size_t index;
} VecIter;

// Helper Functions
void    *Vec_get_pointer(Vec *vec, size_t index);
void    *Vec_pop_pointer(Vec *vec);
bool    Vec_resize(Vec *vec);

// Generic functions

Vec *Vec_with_capacity(size_t capacity)
{
    Vec *init = malloc(sizeof *init);
    if (!init)
    {
        return NULL;
    }

    init->arr = calloc(capacity, sizeof(void **));
    if (!init->arr)
    {
        return NULL;
    }

    init->capacity = capacity;
    init->length = 0;
    return init;
}

Vec *Vec_new()
{
    return Vec_with_capacity(DEFAULT_CAPACITY);
}

void Vec_delete(Vec **self)
{
    if (*self == NULL)
        return;

    free((*self)->arr);
    free(*self);
}

bool Vec_resize(Vec *self)
{
    void *temp;
    size_t new_capacity;
    if (self->capacity < 2)
    {
        new_capacity = 4;
    }
    else if (self->capacity == 3)
    {
        new_capacity = 8;
    }
    else
    {
        new_capacity = self->capacity * RESIZE_CONSTANT;
    }
    temp = realloc(self->arr, new_capacity * sizeof(void **));
    if (temp == NULL)
    {
        return false;
    }
    self->arr = temp;
    self->capacity = new_capacity;
    return true;
}

bool Vec_push(Vec *self, void *item)
{
    if (self->length >= self->capacity)
    {
        if (Vec_resize(self) == false) 
        {
            return false;
        }
    }
    self->arr[self->length] = item;
    self->length++;

    return true;
}

void *Vec_get_pointer(Vec *self, size_t index)
{
    if (index < self->length)
    {
        return self->arr[index];
    }
    else 
    {
        return NULL;
    }
}

Option *Vec_get(Vec *self, size_t index)
{
    void *ret = Vec_get_pointer(self, index);
    if (ret == NULL)
    {
        return Option_of(NULL);
    }
    else 
    {
        return Option_of(ret);
    }
}

void *Vec_pop_pointer(Vec *self) 
{
    void *ret = NULL;
    if (self->length > 0)
    {
        ret = self->arr[self->length - 1];
        self->length--;
    }
    return ret;
}

Option *Vec_pop(Vec *self)
{
    void *ret = Vec_pop_pointer(self);
    if (ret == NULL)
    {
        return Option_of(NULL);
    }
    else 
    {
        return Option_of(ret);
    }
}

size_t Vec_length(Vec *self)
{
    return self->length;
}

bool Vec_empty(Vec *self)
{
    return (Vec_length(self) == 0);
}


// -------------- Vec Constructor Stuff ----------------------------------- //

bool VecConstr_push(void *vec, void *item)
{
    return Vec_push(vec, item);
}

static const ConstructorVTable constr_vtable = {
    .push = VecConstr_push,
};

Constructor Vec_constr()
{
    Vec *vec = Vec_new();
    Constructor constr = {
        .vtable = &constr_vtable,
        .collection = vec,
    };
    return constr;
}

// -------------- Vec Iterator stuff -------------------------------------- //

Option *VecIter_next(void *self_as_void_ptr)
{
    VecIter *self = self_as_void_ptr;
    Option *ret = Vec_get(self->vec, self->index);
    self->index++;
    return ret;
}

Option *VecIterRev_next(void *self_as_void_ptr)
{
    VecIter *self = self_as_void_ptr;
    if (self->vec->length == 0)
    {
        return Option_of(NULL);
    }
    Option *ret = Vec_get(self->vec, self->vec->length - 1 - self->index);
    self->index++;
    return ret;
}

unsigned long VecIter_length(void *self_as_void_ptr)
{
    VecIter *self = self_as_void_ptr;
    return self->vec->length;
}

void *Vec_into_iter(void *self_as_void_ptr)
{
    Vec *self = self_as_void_ptr;
    VecIter *iter = malloc(sizeof *iter);
    iter->index = 0; 
    iter->vec = self;
    return iter;
}

void VecIter_delete(void *self_as_void_ptr, bool delete_collection)
{
    VecIter *self = self_as_void_ptr;
    if (delete_collection)
    {
        Vec_delete(&self->vec);
    }
    self->vec = NULL;
    free(self);
}

IteratorVTable Vec_iter(Vec *self, bool delete_collection_after_iter)
{
    IteratorVTable table;
    table.iterable_object = Vec_into_iter(self);
    table.next = ^ Option *(void *self_as_void_ptr) 
    { 
        return VecIter_next(self_as_void_ptr); 
    };
    table.delete_iter = VecIter_delete;
    table.delete_collection_after_iter = delete_collection_after_iter;
    return table;
}

IteratorVTable Vec_iter_reverse(Vec *self, bool delete_collection_after_iter)
{
    IteratorVTable table;
    table.iterable_object = Vec_into_iter(self);
    table.next = ^ Option *(void *self_as_void_ptr) 
    { 
        return VecIterRev_next(self_as_void_ptr); 
    };
    table.delete_iter = VecIter_delete;
    table.delete_collection_after_iter = delete_collection_after_iter;
    return table; 
} 

////////////////////////////////////////////////////////////// 
//                                                          //
//  For convienience, we defined many versions of vector    //
//  for various primative types.                            // 
//  Each version is its own struct, only                    //
//  holding the instance of the generic vector itself,      //
//  so that the different versions of the vector can't      //
//  be substituted for one another. This creates a bit      //
//  more type-safety than to use typedefs to create the     //
//  other versions.                                         //
//                                                          //
//////////////////////////////////////////////////////////////

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;
typedef double f64;

#define VEC_TYPE_FACTORY(T) \
struct __Vector_##T##_Struct { \
    size_t capacity; \
    size_t length; \
    T *arr; \
};

#define VEC_TYPE_FN_FACTORY(T) \
bool    Vec_##T##_resize(Vec_##T *vec); \
Vec_##T *Vec_##T##_with_capacity(size_t capacity) { \
    Vec_##T *init = malloc(sizeof *init); \
    if (!init) { \
        return NULL; \
    } \
    init->arr = calloc(capacity, sizeof(T)); \
    if (!init->arr) { \
        return NULL; \
    } \
    init->capacity = capacity; \
    init->length = 0; \
    return init; \
} \
Vec_##T *Vec_##T##_new() { \
    return Vec_##T##_with_capacity(DEFAULT_CAPACITY); \
} \
void Vec_##T##_delete(Vec_##T **self) { \
    if (*self == NULL) \
        return; \
    free((*self)->arr); \
    free(*self); \
} \
bool Vec_##T##_resize(Vec_##T *self) { \
    void *temp; \
    size_t new_capacity; \
    if (self->capacity < 2) { \
        new_capacity = 4; \
    } \
    else if (self->capacity == 3) { \
        new_capacity = 8; \
    } \
    else { \
        new_capacity = self->capacity * RESIZE_CONSTANT; \
    } \
    temp = realloc(self->arr, new_capacity * sizeof(T)); \
    if (temp == NULL) { \
        return false; \
    } \
    self->arr = temp; \
    self->capacity = new_capacity; \
    return true; \
} \
bool Vec_##T##_push(Vec_##T *self, T item) { \
    if (self->length >= self->capacity) { \
        if (Vec_##T##_resize(self) == false) { \
            return false; \
        } \
    } \
    self->arr[self->length] = item; \
    self->length++; \
    return true; \
} \
Option_##T *Vec_##T##_get(Vec_##T *self, size_t index) { \
    if (index < self->length) { \
        return Option_##T##_of(self->arr[index]); \
    } \
    else { \
        return Option_##T##_empty(); \
    } \
} \
Option_##T *Vec_##T##_pop(Vec_##T *self) { \
    if (self->length == 0) { \
        return Option_##T##_empty(); \
    } \
    else { \
        self->length--; \
        return Option_##T##_of(self->arr[self->length]); \
    } \
} \
size_t Vec_##T##_length(Vec_##T *self) { \
    return self->length; \
} \
bool Vec_##T##_empty(Vec_##T *self) { \
    return (Vec_##T##_length(self) == 0); \
} \


VEC_TYPE_FACTORY(i32);
VEC_TYPE_FN_FACTORY(i32);
