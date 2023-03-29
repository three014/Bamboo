#ifndef __SET_H
#define __SET_H
    
#include "interface/iterator.h"
#include "option/option.h"

typedef struct __SET_STRUCT
{
    IteratorVTable (*union_of)(const void *set, const void *other_set);
    IteratorVTable (*intersection_of)(const void *set, const void *other_set);
    IteratorVTable (*difference_of)(const void *set, const void *other_set);
    bool (*equal)(const void *set, const void *other_set);
    IteratorVTable (*iter)(const void *set);
    bool (*contains)(const void *set, const void *value);
    Option *(*pop)(void *set);
    bool (*push)(void *set, void *item);
    Option *(*remove)(void *set, const void *item); 
    void *set;
} SetVTable;

#endif // !__SET_H
