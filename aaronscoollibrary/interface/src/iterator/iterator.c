#include "interface/iterator.h"
#include "option/option.h"
#include "vector/vector.h"
#include <stdio.h>
#include <Block.h>

Option *_next_item(const IteratorVTable *table)
{
    Option *next = table->next(table->iterable_object);
    // printf("Returned value inside option inside _next_item: data = %p\n", Option_get(Option_clone(next)));
    return next;
}

void _delete_iter(IteratorVTable *table, bool delete_collection)
{
    table->delete_iter(table->iterable_object, delete_collection);
    table->iterable_object = NULL;
}

void _delete_next_block(IteratorVTable *table)
{
    Block_release(table->next);
}

void Iter_for_each_enumerate(IteratorVTable iter_vtable, void (^consumer)(const unsigned long index, void *item))
{
    if (iter_vtable.iterable_object == NULL) return;
    Option *option = _next_item(&iter_vtable);
    unsigned long i = 0;
    while (Option_is_some(option))
    {
        void *inner = Option_get(option);
        consumer(i++, inner);
        option = _next_item(&iter_vtable);;
    }
    Option_delete(option);
    _delete_iter(&iter_vtable, iter_vtable.delete_collection_after_iter);
    _delete_next_block(&iter_vtable);
}


void Iter_for_each(IteratorVTable iter_vtable, void (^consumer)(void *item))
{
    if (iter_vtable.iterable_object == NULL) return;
    Option *option = _next_item(&iter_vtable);
    while (Option_is_some(option))
    {
        void *inner = Option_get(option);
        consumer(inner);
        option = _next_item(&iter_vtable);
    }
    Option_delete(option);
    _delete_iter(&iter_vtable, iter_vtable.delete_collection_after_iter);
    _delete_next_block(&iter_vtable);
}

void Iter_for_each_zip(IteratorVTable iter_vtable, IteratorVTable other_iter_vtable, void (^consumer)(void *item, void *other_item))
{
    if (iter_vtable.iterable_object == NULL || other_iter_vtable.iterable_object == NULL) return;
    Option *option = _next_item(&iter_vtable);
    Option *other_option = _next_item(&other_iter_vtable); // FIXME: THIS IS WHERE WE GET THE WRONG ADDR
    while (Option_is_some(option) && Option_is_some(other_option))
    {
        void *inner = Option_get(option);
        void *other_inner = Option_get(other_option);
        // printf("In iter (1): %p\n", inner);
        // printf("In iter (2): %p\n", other_inner);
        consumer(inner, other_inner);
        option = _next_item(&iter_vtable);
        other_option = _next_item(&other_iter_vtable);
    }
    Option_delete(option);
    Option_delete(other_option);
    _delete_iter(&iter_vtable, iter_vtable.delete_collection_after_iter);
    _delete_iter(&other_iter_vtable, other_iter_vtable.delete_collection_after_iter);
    _delete_next_block(&iter_vtable);
    _delete_next_block(&other_iter_vtable);
}

IteratorVTable Iter_map(IteratorVTable iter_vtable, void *(^map_func)(void *item))
{
    Option *(^next_item)(void *iterable) = iter_vtable.next;
    Option *(^map)(void *item) = ^ Option *(void *item) 
    { 
        return Option_of(map_func(item)); 
    };
    iter_vtable.next = Block_copy(^ Option *(void *iterable_object) 
    {
        return Option_map(next_item(iterable_object), map);
    });
    return iter_vtable;
}


IteratorVTable Iter_filter(IteratorVTable iter_vtable, bool (^predicate_func)(void *item))
{
    Option *(^next_item)(void *iterable) = iter_vtable.next;
    iter_vtable.next = Block_copy(^ Option *(void *iterable_object) 
    {
        Option *next = next_item(iterable_object);
        while (!Option_is_some_and(next, predicate_func))
        {
            if (!Option_is_some(next))
            {
                return next;
            }
            Option_delete(next);
            next = next_item(iterable_object);
        }
        return next;
    });
    return iter_vtable;

}
