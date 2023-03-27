#ifndef __AVLTREE_H
#define __AVLTREE_H

#include <stdlib.h>
#include <stdbool.h>
 
#include "option/option.h"
#include "vector.h"
#include "interface/ordering.h"

typedef struct __AvlNode_Struct AvlNode;
typedef struct __AvlTree_Struct AvlTree;
typedef struct __AvlTreeSet_Struct AvlTreeSet;

/* TreeSet Functions */
ConstructorVTable   *AvlTreeSet_constr(OrderingVTable *ordering);
AvlTreeSet          *AvlTreeSet_with_ordering(const OrderingVTable *table);
AvlTreeSet          *AvlTreeSet_new();
bool                AvlTreeSet_is_empty(const AvlTreeSet *self);
bool                AvlTreeSet_contains(const AvlTreeSet *self, const void *item);
bool                AvlTreeSet_insert(const AvlTreeSet *self, void *data);
//Option              *AvlTreeSet_remove(const AvlTreeSet *self, const void *item);
void                AvlTreeSet_delete(AvlTreeSet **self_ref);

/* Iterator Functions */
IteratorVTable *AvlTreeSet_iter(AvlTreeSet *self);

#endif
