#ifdef __OPTION_TESTS
#include "../test/option.h"
#include "../test/munit.h"
#endif

#ifndef __OPTION_TESTS
#include "option/option_obj.h"
#include "object.h"
#endif
#include "option_template.h"

typedef ObjWrap obj; 

OPTION_TYPE_FACTORY(obj);

Option_obj *Option_obj_of(ObjWrap item) 
{ 
    Option_obj *ret = malloc(sizeof *ret); 
    ret->item = item; 
    ret->some = true; 
    return ret; 
} 
Option_obj *Option_obj_empty() 
{ 
    Option_obj *ret = malloc(sizeof *ret); 
    ret->some = false;
    ret->item = Obj_from_u32(0);
    return ret; 
} 

void Option_obj_delete(Option_obj *self) 
{ 
    free(self); 
}

Option_obj *Option_obj_clone(Option_obj *self) 
{ 
    if (Option_obj_is_some(self)) {
        return Option_obj_of(self->item); 
    } else {
        return Option_obj_empty();
    }
} 

bool Option_obj_is_some(Option_obj *self) 
{ 
    if (self == NULL) 
    { 
        return false; 
    } 
    return self->some; 
} 

bool Option_obj_is_some_and(Option_obj *self, bool (^predicate)(ObjWrap item)) 
{ 
    if (!Option_obj_is_some(self) 
            || predicate == NULL) 
    { 
        return false; 
    } 
    return predicate(self->item); 
} 

Option_obj *Option_obj_take(Option_obj *self) 
{ 
    if (!Option_obj_is_some(self)) 
    { 
        return Option_obj_empty(); 
    } 
    ObjWrap ret = self->item;
    self->item = Obj_from_u32(0); 
    self->some = false; 
    return Option_obj_of(ret); 
} 

bool Option_obj_insert(Option_obj *self, ObjWrap item) 
{ 
    if (Option_obj_is_some(self)) 
    { 
        return false; 
    } 
    else 
    { 
        self->item = item; 
        self->some = true; 
    } 
    return true; 
} 

Option_obj *Option_obj_replace(Option_obj *self, ObjWrap item) 
{ 
    if (Option_obj_is_some(self)) 
    { 
        Option_obj *ret = Option_obj_of(self->item); 
        self->item = item; 
        return ret; 
    } 
    else 
    { 
        Option_obj *ret = Option_obj_empty(); 
        self->item = item; 
        return ret; 
    } 
}

ObjWrap Option_obj_get(Option_obj *self) 
{ 
    if (!Option_obj_is_some(self)) return Obj_from_u32(0); 
    ObjWrap ret = self->item;
    self->item = Obj_from_u32(0); 
    Option_obj_delete(self); 
    return ret; 
} 

ObjWrap Option_obj_get_or_else(Option_obj *self, ObjWrap (^default_func)()) 
{ 
    if (Option_obj_is_some(self)) 
    { 
        return Option_obj_get(self); 
    } 
    else if (default_func == NULL) 
    { 
        Option_obj_delete(self); 
        return Obj_from_u32(0); 
    } 
    else 
    { 
        Option_obj_delete(self); 
        return default_func(); 
    } 
} 

ObjWrap Option_obj_get_or(Option_obj *self, ObjWrap default_value) 
{ 
    if (Option_obj_is_some(self)) 
    {
        return Option_obj_get(self); 
    } 
    else 
    { 
        Option_obj_delete(self); 
        return default_value; 
    } 
} 

