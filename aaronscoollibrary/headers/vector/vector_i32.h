#ifndef __VECTOR_I32_H
#define __VECTOR_I32_H

#include "option/option.h"

typedef struct __Vector_i32_Struct Vec_i32;

Vec_i32         *Vec_i32_new();
Vec_i32         *Vec_i32_with_capacity(size_t capacity);
void            Vec_i32_delete(Vec_i32 **self);
unsigned long   Vec_i32_length(Vec_i32 *self);
bool            Vec_i32_push(Vec_i32 *self, int32_t item);
Option_i32      *Vec_i32_pop(Vec_i32 *self);
Option_i32      *Vec_i32_get(Vec_i32 *self, size_t index);
bool            Vec_i32_empty(Vec_i32 *self);


#endif // !__VECTOR_I32_H
