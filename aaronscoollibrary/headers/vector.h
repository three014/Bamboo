#ifndef __VECTOR_H
#define __VECTOR_H

#include "option.h"
#include "interface/iterator.h"

#include <stdbool.h>

typedef struct __Vector_Struct Vec;
typedef struct __Vector_i32_Struct Vec_i32;
typedef struct __Vector_u32_Struct Vec_u32;

Vec                     *Vec_new();
Vec                     *Vec_with_capacity(size_t capacity);
void                    Vec_delete(Vec *self);
unsigned long           Vec_length(Vec *self);
bool                    Vec_push(Vec *self, void *item);
Option                  *Vec_pop(Vec *self);
Option                  *Vec_get(Vec *self, size_t index);
bool                    Vec_empty(Vec *self);
ConstructorVTable       *Vec_constr();

/* Iterator Functions */
IteratorVTable          *Vec_iter(Vec *self);
IteratorVTable          *Vec_iter_reverse(Vec *self);

#endif
