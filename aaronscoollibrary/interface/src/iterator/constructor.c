#include "interface/iterator.h"
#include <stdio.h>

void *Iter_collect(IteratorVTable old_iter_vtable, Constructor new_collection_constructor)
{
    Iter_for_each(old_iter_vtable, ^ void (void *item) 
    {
        new_collection_constructor.vtable->push(new_collection_constructor.collection, item);
    });

    return new_collection_constructor.collection;
}

