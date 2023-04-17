#include "interface/iterator.h"
#include "object.h"
#include "../interface/src/object/object_p.h"
#include "option/option.h"
#include "option/option_obj.h"
#include "vector/vector.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


#define DEFAULT_CAPACITY 8
#define RESIZE_CONSTANT 2

typedef struct __Vector_Improved_Struct Vec;

struct __Vector_Improved_Struct {
    size_t capacity;
    size_t length;
    ObjectInternal *arr;
};

typedef struct __Vector_Improved_Iterator {
    struct __Vector_Improved_Struct *vec;
    size_t index;
} VecIter;


bool _resize_vec(struct __Vector_Improved_Struct *vec);

Vec *Vec_with_capacity(size_t capacity) {
    Vec *init = malloc(sizeof *init);
    if (!init) {
        return NULL;
    }

    init->arr = NULL;
    init->capacity = capacity;
    init->length = 0;
    return init;
}

Vec *Vec_new() {
    return Vec_with_capacity(DEFAULT_CAPACITY);
}

void Vec_delete(Vec **self) {
    if (*self == NULL)
        return;

    free((*self)->arr);
    free(*self);
}


bool _resize_vec(Vec *self) {
    if (self->arr == NULL) {
        self->arr = calloc(self->capacity, sizeof(ObjectInternal));
        return (self->arr != NULL);
    }

    void *temp;
    size_t new_capacity;
    if (self->capacity < 2) {
        new_capacity = 4;
    }
    else if (self->capacity == 3) {
        new_capacity = 8;
    }
    else {
        new_capacity = self->capacity * RESIZE_CONSTANT;
    }
    temp = realloc(self->arr, new_capacity * sizeof(ObjectInternal));
    if (temp == NULL) {
        return false;
    }
    self->arr = temp;
    self->capacity = new_capacity;
    return true;
}


bool Vec_push(Vec *self, ObjWrap item) {
    if (self->length >= self->capacity || self->arr == NULL) {
        if (_resize_vec(self) == false) {
            return false;
        }
    }
    ObjectInternal converted_item = ObjWrap_to_internal(item);

    self->arr[self->length] = converted_item;
    self->length++;

    return true;
}

Option_obj *Vec_get(Vec *self, size_t index) {
    Option_obj *ret = Option_obj_empty();
    if (index < self->length) {
        ObjWrap item = ObjectInternal_to_wrap(self->arr[index]);
        Option_obj_insert(ret, item);
    }
    return ret;
}

Option_obj *Vec_pop(Vec *self) {
    Option_obj *ret = Option_obj_empty();
    if (self->length > 0) {
        ObjWrap item = ObjectInternal_to_wrap(self->arr[self->length - 1]);
        self->length--;
        Option_obj_insert(ret, item);
    }
    return ret;
}

size_t Vec_length(Vec *self) {
    return self->length;
}

bool Vec_empty(Vec *self) {
    return (Vec_length(self) == 0);
}

void Vec_clear(Vec *self, void (^delete_value_func)(void *item)) {
    if (delete_value_func != NULL) {
        Iter_for_each_obj(Vec_iter(self, false), ^ void (ObjWrap item) {
            delete_value_func(Obj_to_ptr(item));
        });
    }
    self->length = 0;
}

void Vec_deep_delete(Vec *self, void (^delete_value_func)(void *item)) {
    Vec_clear(self, delete_value_func);
    Vec_delete(&self);
}


// -------------- Vec Iterator stuff -------------------------------------- //

Option_obj *VecIter_next(void *self_as_void_ptr) {
    VecIter *self = self_as_void_ptr;
    Option_obj *ret = Option_obj_empty();
    if (self->vec->length == 0) {
        return ret;
    }
    if (self->index < self->vec->length) {
        Option_obj_insert(ret, ObjectInternal_to_wrap(self->vec->arr[self->index]));
        self->index++;
        // if (self->vec->arr[self->index - 1].is_ptr) {
        //     void *test = Obj_to_ptr(ObjectInternal_to_wrap(self->vec->arr[self->index - 1]));
        //     printf("inside VecIter_next, checking ptr: %p\n", test);
        // }
    }

    // printf("still inside VecIter_next, checking if opt is some at end of function: %d\n", Option_obj_is_some(ret));
    return ret;
}

Option_obj *VecIterRev_next(void *self_as_void_ptr) {
    VecIter *self = self_as_void_ptr;
    Option_obj *ret = Option_obj_empty();
    if (self->vec->length == 0) {
        return ret;
    }
    if (self->vec->length - 1 - self->index < self->vec->length) {
        ObjWrap item = ObjectInternal_to_wrap(self->vec->arr[self->vec->length - 1 - self->index]);
        Option_obj_insert(ret, item);
        self->index++;
    }
    return ret;
}

void *Vec_into_iter(void *self_as_void_ptr) {
    Vec *self = self_as_void_ptr;
    VecIter *iter = malloc(sizeof *iter);
    iter->index = 0; 
    iter->vec = self;
    return iter;
}

void VecIter_delete(void *self_as_void_ptr, bool delete_collection) {
    VecIter *self = self_as_void_ptr;
    if (delete_collection) {
        Vec_delete(&self->vec);
    }
    self->vec = NULL;
    free(self);
}

static const IteratorVTable Vec_iter_vtable = {
    .delete_iterable_obj = ^ void (void *self_as_void_ptr, bool delete_collection) {
        VecIter_delete(self_as_void_ptr, delete_collection);
    },
    .next = ^ Option_obj *(void *self_as_void_ptr) {
        return VecIter_next(self_as_void_ptr);
    }
};

Iterator Vec_iter(Vec *self, bool delete_collection_after_iter) {
    return (Iterator) {
        .vtable = Vec_iter_vtable,
        .delete_collection_after_iter = delete_collection_after_iter,
        .iterable_obj = Vec_into_iter(self)
    };
}

Iterator Vec_iter_reverse(Vec *self, bool delete_collection_after_iter) {
    return (Iterator) {
        .vtable = (IteratorVTable) {
            .next = ^ Option_obj *(void *self_as_void_ptr) {
                return VecIterRev_next(self_as_void_ptr);
            },
            .delete_iterable_obj = ^ void (void *self_as_void_ptr, bool delete_collection) {
                VecIter_delete(self_as_void_ptr, delete_collection);
            }
        },
        .delete_collection_after_iter = delete_collection_after_iter,
        .iterable_obj = Vec_into_iter(self)
    };
} 

// ----------- Vec Constructor stuff ------------------------------------------- //

bool _vec_constructor_push(void *collection, ObjWrap item) { return Vec_push(collection, item); }

static const ConstructorVTable Vec_constr_vtable = {
    .push = _vec_constructor_push
};


Constructor Vec_constr() {
    return (Constructor) {
        .collection = Vec_new(),
        .vtable = &Vec_constr_vtable
    };
}

Constructor Vec_constr_with_capacity(size_t capacity) {
    return (Constructor) {
        .collection = Vec_with_capacity(capacity),
        .vtable = &Vec_constr_vtable
    };
}
