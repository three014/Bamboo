#include "avltreemap.h"
#include <stdio.h>
#include <string.h>

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

MapInternalKV MapKV_to_internal(MapKV kv);

int main() {
    int x = 5;
    int y = 32;
    MapKV kv = { .hidden = { 0 } };
    memcpy(kv.hidden, &x, sizeof(int32_t));
    memcpy((kv.hidden + sizeof(size_t)), &y, sizeof(int32_t));
    MapInternalKV ikv = MapKV_to_internal(kv);
    printf("%d\n", (int) ikv.key.k);
    printf("%d\n", (int) ikv.value.v);

    int64_t a = -123;
    char *b = "Hello how it do?";
    MapKV kv1 = {.hidden = {0}};
    memcpy(kv1.hidden, &a, sizeof(int64_t));
    memcpy((kv1.hidden + sizeof(size_t)), &b, sizeof(char *));
    kv1.hidden[17] = 1;
    MapInternalKV ikv1 = MapKV_to_internal(kv1);
    printf("%ld\n", (int64_t) ikv1.key.k);
    printf("%s\n", *(char **) &ikv1.value.v);

    int8_t c = 3;
    double d = 423.345123;
    MapKV kv2 = {.hidden = {0}};
    memcpy(kv2.hidden, &c, sizeof(int8_t));
    memcpy(kv2.hidden + sizeof(size_t), &d, sizeof(double));
    MapInternalKV ikv2 = MapKV_to_internal(kv2);
    printf("%d\n", *(int8_t *) &ikv2.key.k);
    printf("%lf\n", *(double *) &ikv2.value.v);

    return 0;
}




MapInternalKV MapKV_to_internal(MapKV kv) {
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


