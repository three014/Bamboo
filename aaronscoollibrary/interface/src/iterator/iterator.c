#include "interface/iterator.h"
#include "option/option.h"
#include "vector/vector.h"
#include <stdio.h>
#include <Block.h>


// void Iter_for_each_enumerate(IteratorVTable iter_vtable, void (^consumer)(const unsigned long index, void *item))
// {
//     if (iter_vtable.iterable_object == NULL) return;
//     Option *option = _next_item(&iter_vtable);
//     unsigned long i = 0;
//     while (Option_is_some(option))
//     {
//         void *inner = Option_get(option);
//         consumer(i++, inner);
//         option = _next_item(&iter_vtable);;
//     }
//     Option_delete(option);
//     _delete_iter(&iter_vtable, iter_vtable.delete_collection_after_iter);
//     _delete_next_block(&iter_vtable);
// }
//



Iterator Iter_map(Iterator iter, ObjWrap (^map_func)(ObjWrap item)) {
    Option_obj *(^next_item)(void *iterable) = iter.vtable.next;
    iter.vtable.next = Block_copy(^ Option_obj *(void *iterable_object) {
        Option_obj *opt = next_item(iterable_object);
        if (Option_obj_is_some(opt)) {
            ObjWrap item = Option_obj_get(Option_obj_take(opt));
            // printf("inside iter_map: %lu\n", *(size_t *) &item); // At this point, the objwrap is set to all zeroes. Why? Shouldn't that only happen if Option isn't some?
                                                                 // Semi-answer: apparently, if option_obj is not some, then option_obj_get gives us a 0 in the form of a ObjWrap
                                                                 // Either that, or the actual value inside was somehow a 0 (shouldn't happen)
            Option_obj_insert(opt, map_func(item));
        }
        return opt;
    });
    return iter;
}

Option_obj *_next_obj_item(Iterator *iter_ref) {
    return iter_ref->vtable.next(iter_ref->iterable_obj);
}

void _delete_obj_iter(Iterator *iter_ref, bool delete_collection) {
    iter_ref->vtable.delete_iterable_obj(iter_ref->iterable_obj, delete_collection);
    iter_ref->iterable_obj = NULL;
}

void _delete_next_block_for_obj_iter(Iterator *iter_ref) {
    Block_release(iter_ref->vtable.next);
}

void Iter_for_each_obj(Iterator iter, void (^consumer)(ObjWrap item)) {
    if (iter.iterable_obj == NULL) return;
    Option_obj *opt = _next_obj_item(&iter);
    while (Option_obj_is_some(opt)) {
        ObjWrap inner = Option_obj_get(opt);
        consumer(inner);
        opt = _next_obj_item(&iter);
    }
    Option_obj_delete(opt);
    _delete_obj_iter(&iter, iter.delete_collection_after_iter);
    _delete_next_block_for_obj_iter(&iter);
}

// void Iter_for_each_zip(Iterator iter_vtable, Iterator other_iter_vtable, void (^consumer)(ObjWrap item, ObjWrap other_item))
// {
//     if (iter_vtable.iterable_object == NULL || other_iter_vtable.iterable_object == NULL) return;
//     Option *option = _next_item(&iter_vtable);
//     Option *other_option = _next_item(&other_iter_vtable); // FIXME: THIS IS WHERE WE GET THE WRONG ADDR
//     while (Option_is_some(option) && Option_is_some(other_option))
//     {
//         void *inner = Option_get(option);
//         void *other_inner = Option_get(other_option);
//         // printf("In iter (1): %p\n", inner);
//         // printf("In iter (2): %p\n", other_inner);
//         consumer(inner, other_inner);
//         option = _next_item(&iter_vtable);
//         other_option = _next_item(&other_iter_vtable);
//     }
//     Option_delete(option);
//     Option_delete(other_option);
//     _delete_iter(&iter_vtable, iter_vtable.delete_collection_after_iter);
//     _delete_iter(&other_iter_vtable, other_iter_vtable.delete_collection_after_iter);
//     _delete_next_block(&iter_vtable);
//     _delete_next_block(&other_iter_vtable);
// e
