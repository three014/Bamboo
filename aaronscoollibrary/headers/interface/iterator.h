#ifndef __ITERATOR_H
#define __ITERATOR_H

#include "option/option.h"

#include <stdlib.h>

typedef struct __Obj_Iterator_VTable_Struct {
    Option_obj *(^next)(void *iterable_object);
    void (^delete_iterable_obj)(void *iterable_obj, bool delete_collection);
} IteratorVTable;

typedef struct __Iterator_Struct {
    IteratorVTable vtable;
    void *iterable_obj;
    bool delete_collection_after_iter;
} Iterator;


typedef struct __Construct_Collection_VTable_Struct {
    bool (*push)(void *collection, ObjWrap item_from_other_iterator);
} ConstructorVTable;

typedef struct __Constructor_Struct {
    const ConstructorVTable *vtable;
    void *collection;
} Constructor;

Iterator  Iter_map(Iterator iter, ObjWrap (^map_func)(ObjWrap item));
// IteratorVTable  Iter_filter(IteratorVTable iter_vtable, bool (^predicate_func)(void *item));


void            Iter_for_each_obj(Iterator iter, void (^consumer)(ObjWrap item));


void            *Iter_collect(Iterator other_iter, Constructor new_collection_constructor);
// void            Iter_for_each_enumerate(IteratorVTable iter_vtable, 
                        // void (^consumer)(const size_t index, void *item));
// void            Iter_for_each(IteratorVTable iter_vtable, void (^consumer)(void *item));
// void            Iter_for_each_zip(IteratorVTable iter_vtable, IteratorVTable other_iter_vtable, 
                        // void (^consumer)(void *item, void *other_item));

#endif
