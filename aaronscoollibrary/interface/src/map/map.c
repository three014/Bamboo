#include "map_p.h"

#include <string.h>


MapInternalKV MapKV_encode_to_internal(MapEncodeKV kv) {
    size_t key = 0;
    size_t value = 0;
    bool key_is_addr = (kv.hidden[16]) ? true : false;
    bool value_is_addr = (kv.hidden[17]) ? true: false;

    memcpy(&key, kv.hidden, sizeof(size_t));
    memcpy(&value, (kv.hidden + sizeof(size_t)), sizeof(size_t));

    return (MapInternalKV) {
        .key = {
            .is_addr = key_is_addr,
            .k = key
        },
        .value = {
            .is_addr = value_is_addr,
            .v = value
        }
    };
}

MapExternalKV MapKV_internal_to_external(MapInternalKV ikv) {
    void *key;
    void *value;
    if (ikv.key.is_addr) {
        key = *(void **) &ikv.key.k;
    }
    else {
        key = (void *) &ikv.key.k;
    }
    if (ikv.value.is_addr) {
        value = *(void **) &ikv.value.v;
    }
    else {
        value = (void *) &ikv.value.v;
    }
    return (MapExternalKV) {
        .key = key,
        .value = value
    };
}
