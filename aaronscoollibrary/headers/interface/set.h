#ifndef __SET_H
#define __SET_H
    
#include "interface/iterator.h"
#include "interface/ordering.h"
#include "option/option.h"

/**
 * The Set VTable. 
 * Contains all the function pointers for what the set should be able to do.
 *
 * These functions must be implemented for your set data structure and 
 * attached to every instance of this struct, because all functions that 
 * require a Set should have access to every function specified here.
 *
 *
 *
 * The Core Set Functions:
 *
 * These should be supported by all sets. For the implementation,
 * each function should return an IteratorVTable with 
 * "delete_collection_after_iter" set to true, so that users will have to 
 * call Iter_collect() on the iterator to save the output data to a collection.
 */
typedef struct __Set_VTable_Struct
{
    // Core Set Function. 
    // Returns an iterator with all elements in "set" or "other_set".
    Iterator (*union_of)(const void *set, const void *other_set);

    // Core Set Function. 
    // Returns an iterator with all elements found in "set" and "other_set".
    Iterator (*intersection_of)(const void *set, const void *other_set);

    // Core Set Function.
    // Returns an iterator with all elements found in "set" and "other_set", 
    // but not including elements found in both sets.
    Iterator (*difference_of)(const void *set, const void *other_set);

    // Core Set Function.
    // Returns an iterator of the set, specified with an option to delete the 
    // set after running an Iter function on the iterator.
    Iterator (*iter)(void *set, bool delete_collection_after_iter);

    // Returns whether there are no items in the set.
    bool (*empty)(const void *set);

    // Compares two sets, and returns true if and only if all items 
    // in both sets are equal. Does not assume that the items are 
    // sorted or in any specific order, just that both sets contain the 
    // same items.
    bool (*equal)(void *set, void *other_set);

    // Searches for the item in the set, and returns true if found, false
    // if not found.
    bool (*contains)(const void *set, const void *value);

    // Removes the top item from the set and returns an Option of the item.
    // The "top" item is implementation-specific, so it could return any
    // item in the set.
    Option *(*pop)(void *set);

    // Adds an item to the set, returning true if the operation was successful.
    bool (*push)(void *set, void *item);

    // Removes a specific item from the set, returning an Option of the item if
    // it exists. Otherwise returns an empty optional.
    Option *(*remove)(void *set, const void *item); 

    // Deletes the specified set.
    void (*delete)(void *set);
} SetVTable;

typedef struct __Set_Struct
{
    // A pointer to the vtable.
    const SetVTable *vtable;

    // A pointer to the address itself. Take care to not lose access to this memory
    // reference, or to not use any of the operations of the set struct if this 
    // pointer is NULL.
    void *set;
} Set;

#endif // !__SET_H
