#include "avltreemap.h"
#include <stdio.h>

struct MapInternal_KeyValue_Struct {
    const struct Key {
        size_t k;
        bool is_addr;
    } key;
    const struct Value {
        size_t v;
        bool is_addr;
    } value;
};




