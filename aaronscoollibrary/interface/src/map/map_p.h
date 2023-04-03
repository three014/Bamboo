#ifndef __MAP_PRIVATE_H
#define __MAP_PRIVATE_H


/***************************************************************************
 *
 * map_p.h: The Map Interface's Private Header
 *
 * Users implementing a map data structure can use this header to 
 * standardize the way key-value pairs are handled across various types
 * of map collections. The implemented data structure must also satisfy
 * the functions defined in map.h.
 *
 **************************************************************************/

#include "interface/map.h"

#include <stdbool.h>
#include <stddef.h>

typedef struct MapInternal_KeyValue_Struct {
    const struct Key {
        size_t k;
        bool is_addr;
    } key;
    const struct Value {
        size_t v;
        bool is_addr;
    } value;
} MapInternalKV;

/**
 * Converts a byte encoded Key-Value pair object into 
 * the internal representation of the key-values.
 *
 * Both the key and value each can't be longer than 8 bytes,
 * no matter if the object is an actual value or an address to 
 * an object.
 */
MapInternalKV MapKV_encode_to_internal(MapEncodeKV encoded_kv);

/**
 * Converts an internal Key-Value pair object into a pair of void pointers
 * for a user to cast and deref as the expected type.
 *
 * Lifetime of the returned object is shorter than the lifetime of the internal 
 * Key-Value pair object, and shorter than the lifetime of the objects the
 * addresses point to as well.
 */
MapExternalKV MapKV_internal_to_external(MapInternalKV internal_kv);


#endif // !__MAP_PRIVATE_H
