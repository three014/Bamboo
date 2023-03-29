#include "interface/iterator.h"
#include "option/option.h"
#include "vector.h"
#include <stdio.h>
#include <Block.h>

Option *_next_item(const IteratorVTable *table)
{
    return table->next(table->iterable_object);
}

void _delete_iter(IteratorVTable *table)
{
    table->delete_iter(table->iterable_object);
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
    _delete_iter(&iter_vtable);
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
    _delete_iter(&iter_vtable);
    _delete_next_block(&iter_vtable);
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
