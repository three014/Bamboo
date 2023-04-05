#ifndef __OPTION_OBJECT_H
#define __OPTION_OBJECT_H

#include "object.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct __Option_obj_Struct Option_obj;

Option_obj  *Option_obj_of(ObjWrap item);
Option_obj  *Option_obj_empty();
void        Option_obj_delete(Option_obj *self);
Option_obj  *Option_obj_clone(Option_obj *self);
bool        Option_obj_is_some(Option_obj *self);
Option_obj  *Option_obj_take(Option_obj *self);
bool        Option_obj_insert(Option_obj *self, ObjWrap item);
Option_obj  *Option_obj_replace(Option_obj *self, ObjWrap item);
ObjWrap     Option_obj_get(Option_obj *self);
ObjWrap     Option_obj_get_or(Option_obj *self, ObjWrap default_value);
ObjWrap     Option_obj_get_or_else(Option_obj *self, ObjWrap (^default_func)());

#endif
