#ifndef __ORDERING_H
#define __ORDERING_H

typedef enum Ordering_Enum
{
    Less,
    Equal,
    Greater,
} Ordering;

typedef struct __Ordering_VTable_Struct
{
    Ordering (*cmp)(const void *item1, const void *item2);
} OrderingVTable;


#endif
