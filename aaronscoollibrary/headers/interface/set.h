#ifndef __SET_H
#define __SET_H
    
#include "interface/iterator.h"
#include "interface/ordering.h"
#include "option/option.h"

/**
 * The Set Struct. Contains a pointer to the set data structure itself,
 * and function pointers for what the set should be able to do.
 *
 * These functions must be implemented for your set data structure and 
 * attached to every instance of this struct, because all functions that 
 * require a Set should have access to every function specified here.
 */
typedef struct __SET_STRUCT
{
    // Returns an iterator that contains all items from both given sets. 
    IteratorVTable (*union_of)(const void *set, const void *other_set);
    IteratorVTable (*intersection_of)(const void *set, const void *other_set);
    IteratorVTable (*difference_of)(const void *set, const void *other_set);
    IteratorVTable (*iter)(void *set, bool delete_collection_after_iter);
    bool (*empty)(const void *set);
    bool (*equal)(const void *set, const void *other_set);
    bool (*contains)(const void *set, const void *value);
    Option *(*pop)(void *set);
    bool (*push)(void *set, void *item);
    Option *(*remove)(void *set, const void *item); 
    const OrderingVTable *ord;
    void *set;
} SetVTable;

#endif // !__SET_H
