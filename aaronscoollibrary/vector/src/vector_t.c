#include "interface/iterator.h"
#include "object.h"
#include "../interface/src/object/object_p.h"
#include "option/option.h"
#include "option/option_obj.h"

#include <stddef.h>
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
    const void *view;
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


// -------------- Vec Iterator stuff -------------------------------------- //

Option *VecIter_next(void *self_as_void_ptr) {
    VecIter *self = self_as_void_ptr;
    Option *ret = Option_of(NULL);
    if (self->vec->length == 0) {
        return ret;
    }
    if (self->index < self->vec->length) {
        self->view = ObjectInternal_to_view(&self->vec->arr[self->index]);
        Option_insert(ret, (void *) self->view);
        self->index++;
    }
    return ret;
}

Option *VecIterRev_next(void *self_as_void_ptr) {
    VecIter *self = self_as_void_ptr;
    Option *ret = Option_of(NULL);
    if (self->vec->length == 0) {
        return ret;
    }
    if (self->vec->length - 1 - self->index < self->vec->length) {
        self->view = ObjectInternal_to_view(&self->vec->arr[self->vec->length - 1 - self->index]);
        Option_insert(ret, (void *) self->view);
        self->index++;
    }
    return ret;
}

void *Vec_into_iter(void *self_as_void_ptr) {
    Vec *self = self_as_void_ptr;
    VecIter *iter = malloc(sizeof *iter);
    iter->index = 0; 
    iter->vec = self;
    iter->view = NULL;
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

IteratorVTable Vec_iter(Vec *self, bool delete_collection_after_iter) {
    IteratorVTable table;
    table.iterable_object = Vec_into_iter(self);
    table.next = ^ Option *(void *self_as_void_ptr) { 
        return VecIter_next(self_as_void_ptr); 
    };
    table.delete_iter = VecIter_delete;
    table.delete_collection_after_iter = delete_collection_after_iter;
    return table;
}

IteratorVTable Vec_iter_reverse(Vec *self, bool delete_collection_after_iter) {
    IteratorVTable table;
    table.iterable_object = Vec_into_iter(self);
    table.next = ^ Option *(void *self_as_void_ptr) { 
        return VecIterRev_next(self_as_void_ptr); 
    };
    table.delete_iter = VecIter_delete;
    table.delete_collection_after_iter = delete_collection_after_iter;
    return table; 
} 

