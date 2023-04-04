#include "avltreemap.h"
#include "../../interface/src/object/object_p.h"

#include <stdio.h>
#include <string.h>


typedef struct __AvlNode_Struct AvlNode;
typedef struct __AvlTree_Struct AvlTree;

AvlTree *AvlTree_new();
AvlNode *AvlNode_new(ObjectInternal key, ObjectInternal value);
void _delete_tree(AvlTree *tree);
void _delete_node(AvlNode *node);
size_t AvlNode_left_height(const AvlNode *self);
size_t AvlNode_right_height(const AvlNode *self);
int8_t AvlNode_balance_factor(const AvlNode *self);
void AvlNode_update_height(AvlNode *self);
bool AvlNode_rebalance(AvlNode *self);
bool AvlNode_rotate_right(AvlNode *self);
bool AvlNode_rotate_left(AvlNode *self);

struct __AvlTree_Struct {
    // Option<AvlNode<ObjInternal<K>, ObjInternal<V>>>
    Option *avl_node;
};

struct __AvlNode_Struct {
    size_t height;
    AvlTree *left;
    AvlTree *right;
    ObjectInternal key;
    ObjectInternal value;
};

struct __AvlTreeMap_Struct {
    AvlTree *root;    
    const OrderingVTable *ord;
};

// int main() {
//     int x = 5;
//     int y = 32;
//     MapEncodeKV kv = { .hidden = { 0 } };
//     memcpy(kv.hidden, &x, sizeof(int32_t));
//     memcpy((kv.hidden + sizeof(size_t)), &y, sizeof(int32_t));
//     MapInternalKV ikv = MapKV_encode_to_internal(kv);
//     printf("%d\n", (int) ikv.key.k);
//     printf("%d\n", (int) ikv.value.v);
//
//     int64_t a = -123;
//     char *b = "Hello how it do?";
//     MapEncodeKV kv1 = {.hidden = {0}};
//     memcpy(kv1.hidden, &a, sizeof(int64_t));
//     memcpy((kv1.hidden + sizeof(size_t)), &b, sizeof(char *));
//     kv1.hidden[17] = 1;
//     MapInternalKV ikv1 = MapKV_encode_to_internal(kv1);
//     printf("%ld\n", (int64_t) ikv1.key.k);
//     printf("%s\n", *(char **) &ikv1.value.v);
//
//     int8_t c = 3;
//     double d = 423.345123;
//     MapEncodeKV kv2 = {.hidden = {0}};
//     memcpy(kv2.hidden, &c, sizeof(int8_t));
//     memcpy(kv2.hidden + sizeof(size_t), &d, sizeof(double));
//     MapInternalKV ikv2 = MapKV_encode_to_internal(kv2);
//     printf("%d\n", *(int8_t *) &ikv2.key.k);
//     printf("%lf\n", *(double *) &ikv2.value.v);
//
//
//
//
//     return 0;
// }




