#ifndef __ITERATOR_H
#define __ITERATOR_H

#include "option/option.h"

#include <stdlib.h>

typedef struct __Iterator_VTable_Struct
{
    void *iterable_object;
    Option *(^next)(void *iterable_object);
    void (*delete_iter)(void *iterble_object);
    void (*delete_self)(struct __Iterator_VTable_Struct *self);
} IteratorVTable;

typedef struct __Construct_Collection_VTable_Struct
{
    void *collection;
    bool (*Item_push)(void *collection, void *item_from_other_iterator);
    void (*delete_self)(struct __Construct_Collection_VTable_Struct *self);
} ConstructorVTable;

IteratorVTable *Iter_map(IteratorVTable *iter_vtable, void *(^map_func)(void *item));
void *Iter_collect(IteratorVTable *old_iter_vtable, ConstructorVTable *new_collection_constructor_vtable);
void Iter_for_each_enumerate(IteratorVTable *iter_vtable, void (^consumer)(const size_t index, void *item));
void Iter_for_each(IteratorVTable *iter_vtable, void (^consumer)(void *item));

#endif
