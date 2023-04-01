#ifndef __ITERATOR_H
#define __ITERATOR_H

#include "option/option.h"

#include <stdlib.h>

typedef struct __Iterator_VTable_Struct
{
    Option *(^next)(void *iterable_object);
    void (*delete_iter)(void *iterble_object, bool delete_collection);
    bool delete_collection_after_iter;
    void *iterable_object;
} IteratorVTable;

typedef struct __Construct_Collection_VTable_Struct
{
    bool (*push)(void *collection, void *item_from_other_iterator);
} ConstructorVTable;

typedef struct __Constructor_Struct
{
    const ConstructorVTable *vtable;
    void *collection;
} Constructor;

IteratorVTable  Iter_map(IteratorVTable iter_vtable, void *(^map_func)(void *item));
IteratorVTable  Iter_filter(IteratorVTable iter_vtable, bool (^predicate_func)(void *item));
void            *Iter_collect(IteratorVTable old_iter_vtable, 
                        Constructor new_collection_constructor);
void            Iter_for_each_enumerate(IteratorVTable iter_vtable, 
                        void (^consumer)(const size_t index, void *item));
void            Iter_for_each(IteratorVTable iter_vtable, void (^consumer)(void *item));
void            Iter_for_each_zip(IteratorVTable iter_vtable, IteratorVTable other_iter_vtable, 
                        void (^consumer)(void *item, void *other_item));

#endif
