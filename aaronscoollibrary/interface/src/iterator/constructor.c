#include "interface/iterator.h"
#include <stdio.h>

void *Iter_collect(IteratorVTable old_iter_vtable, ConstructorVTable *new_collection_constructor_vtable)
{
    Iter_for_each(old_iter_vtable, ^ void (void *item) 
    {
        new_collection_constructor_vtable->Item_push(new_collection_constructor_vtable->collection, item);
    });
    void *collection = new_collection_constructor_vtable->collection;
    new_collection_constructor_vtable->Self_delete(new_collection_constructor_vtable);

    return collection;
}

