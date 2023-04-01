#include "vector.h"
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
    void **arr;
    size_t capacity;
    size_t length;
};

typedef struct __Vector_Iterator
{
    Vec *vec;
    size_t index;
} VecIter;

// Helper Functions
bool    Vec_push_bytes_from_pointer(Vec *vec, void *item);
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

#define VEC_TYPE_FACTORY(type) \
    typedef struct __Vector_##type##_Struct \
    { \
        Vec vec; \
    } Vec_##type; \

#define VEC_FUNCTION_FACTORY(type) \
Vec_##type *Vec_##type##_new() \
{ \
    return (Vec_##type *) Vec_new(); \
} \
bool Vec_##type##_push(Vec_##type *self, type item) \
{ \
    type *t = malloc(sizeof(type)); \
    *t = item; \
    return Vec_push((Vec *) self, t); \
} \
void Vec_##type##_delete(Vec_##type *self) \
{ \
    Iter_for_each(Vec_iter((Vec *) self, true), ^ void (void *item) \
    { \
        free(item); \
    }); \
}

VEC_TYPE_FACTORY(u32);
VEC_FUNCTION_FACTORY(u32);
