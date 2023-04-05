#ifndef __OBJECT_PRIVATE_H
#define __OBJECT_PRIVATE_H

#include "object.h"

#include <stdbool.h>
#include <stddef.h>

typedef struct __Object_Internal_Struct {
    size_t item;
    bool is_ptr;
} ObjectInternal;

ObjectInternal  ObjWrap_to_internal(ObjWrap item);
void            *ObjectInternal_to_view(const ObjectInternal *internal_item);
ObjWrap         ObjectInternal_to_wrap(ObjectInternal internal_item);





#endif // !__MAP_PRIVATE_H
