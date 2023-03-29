#ifndef __ITERATOR_H
#define __ITERATOR_H

#include "option/option.h"

#include <stdlib.h>

typedef struct __Iterator_VTable_Struct
{
    Option *(^next)(void *iterable_object);
    void (*delete_iter)(void *iterble_object);
    void *iterable_object;
} IteratorVTable;

typedef struct __Construct_Collection_VTable_Struct
{
    bool (*Item_push)(void *collection, void *item_from_other_iterator);
    void (*Self_delete)(struct __Construct_Collection_VTable_Struct *self);
    void *collection;
} ConstructorVTable;

IteratorVTable  Iter_map(IteratorVTable iter_vtable, void *(^map_func)(void *item));
IteratorVTable  Iter_filter(IteratorVTable iter_vtable, bool (^predicate_func)(void *item));
void            *Iter_collect(IteratorVTable old_iter_vtable, ConstructorVTable *new_collection_constructor_vtable);
void            Iter_for_each_enumerate(IteratorVTable iter_vtable, void (^consumer)(const size_t index, void *item));
void            Iter_for_each(IteratorVTable iter_vtable, void (^consumer)(void *item));

#endif
