#ifndef __MAP_H
#define __MAP_H

#include "interface/iterator.h"
#include "object.h"
#include "option/option.h"

#include <stddef.h>
#include <stdint.h>

typedef struct __Map_VTable_Struct {
    IteratorVTable (*iter)(void *map, bool delete_collection_after_iter);
    IteratorVTable (*iter_keys)(void *map, bool delete_collection_after_iter);
    IteratorVTable (*iter_values)(void *map, bool delete_collection_after_iter);
    bool (*push)(void *map, ObjWrap key, ObjWrap value);
    Option_obj *(*view_obj)(const void *map, ObjWrap key);
    Option_obj *(*remove_obj)(void *map, ObjWrap key);
    Option_obj *(*replace_obj)(void *map, ObjWrap key, ObjWrap value);
    bool (*empty)(const void *map);
    bool (*contains_key)(const void *map, ObjWrap key);
    void (*delete_map)(void *map);
} MapVTable;

typedef struct __Map_Struct {
    const MapVTable *vtable;
    void *map;
} Map;

typedef struct __Map_KeyValuePair_Struct {
    const void *key;
    const void *value;
} MapViewKV;

// Thinking: Get rid of all of this, use Object way to store items
// But how to get it out of there properly? Currently writing code
// for object to convert to and from specific types
//
//
// This works for inputing keys/values:
//
// void accept(MapKV pair);
// void foo() {
//     accept((MapKV){Obj_from_str("Hello!"), Obj_from_u16(34)});
// }
//
// Kind of syntax-heavy, might be better to use:
//
// void accept(ObjWrap key, ObjWrap value);
// Option_obj *view_obj(ObjWrap key);
// void foo() {
//     Map m = AvlTreeMap_as_map(AvlTreeMap_with_ordering(&ordering_i32));
//     accept(Obj_from_str("Aaron"), Obj_from_u32(20));
//     uint32_t x = Obj_to_u32(Option_obj_get(m.vtable->view_obj(m.map, Obj_from_str("Aaron"))));
//     uint32_t y = MAP_GET_OR(m, str, "Aaron", u32, 0);
//     uint32_t z = MAP_GET_OR_ELSE(m, str, "Aaron", u32, {
//         return 1 + 2;
//     });
// } 


#endif // !__MAP_H
