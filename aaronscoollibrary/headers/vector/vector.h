#ifndef __VECTOR_H
#define __VECTOR_H

#include "option/option.h"
#include "interface/iterator.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct __Vector_Improved_Struct Vec;

Vec             *Vec_new();
Vec             *Vec_with_capacity(size_t capacity);
void            Vec_delete(Vec **self);
unsigned long   Vec_length(Vec *self);
bool            Vec_push(Vec *self, ObjWrap item);
Option_obj      *Vec_pop(Vec *self);
Option_obj      *Vec_get(Vec *self, size_t index);
bool            Vec_empty(Vec *self);
void            Vec_clear(Vec *self, void (^delete_value_if_ptr_func)(void *item_ptr));
void            Vec_deep_delete(Vec *self, void (^delete_value_if_ptr_func)(void *item_ptr));
Constructor     Vec_constr();
Constructor     Vec_constr_with_capacity(size_t capacity);

/* Iterator Functions */
Iterator  Vec_iter(Vec *self, bool delete_collection_after_iter);
Iterator  Vec_iter_reverse(Vec *self, bool delete_collection_after_iter);

#endif
