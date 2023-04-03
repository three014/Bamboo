#ifndef __AVLTREESET_H
#define __AVLTREESET_H

#include <stdlib.h>
#include <stdbool.h>
 
#include "interface/set.h"
#include "option/option.h"
#include "interface/ordering.h"
#include "interface/iterator.h"

typedef struct __AvlTreeSet_Struct AvlTreeSet;

/* TreeSet Functions */
Constructor     AvlTreeSet_constr(OrderingVTable *ordering);
AvlTreeSet      *AvlTreeSet_with_ordering(const OrderingVTable *table);
AvlTreeSet      *AvlTreeSet_new();
bool            AvlTreeSet_is_empty(const AvlTreeSet *self);
bool            AvlTreeSet_contains(const AvlTreeSet *self, const void *item);
bool            AvlTreeSet_insert(const AvlTreeSet *self, void *data);
Option          *AvlTreeSet_remove(const AvlTreeSet *self, const void *item);
void            AvlTreeSet_delete(AvlTreeSet **self);
IteratorVTable  AvlTreeSet_iter(AvlTreeSet *self, bool delete_collection_after_iter);
Set             AvlTreeSet_as_set(AvlTreeSet *self);

#endif
