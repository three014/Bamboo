#include "avltreemap.h"
#include "../../interface/src/object/object_p.h"
#include "interface/interface_str.h"
#include "vector/vector.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>

// Format: AvlNode<ObjectInternal<K>, ObjectInternal<V>>
typedef struct __AvlNodeKV_Struct AvlNodeKV;

// Format: AvlTree<Option<AvlNode<ObjectInternal<K>, ObjectInternal<V>> *>>
typedef struct __AvlTreeKV_Struct AvlTreeKV;

AvlTreeKV *AvlTreeKV_new();
AvlNodeKV *AvlNodeKV_new(ObjectInternal key, ObjectInternal value);
void _delete_treeKV(AvlTreeKV *tree);
void _delete_nodeKV(AvlNodeKV *node);
Option_obj *_remove_node_from_tree(Vec *prev_tree_ptrs, Option *target_tree_opt, uint8_t *size_decreaser);
Option *_get_node_to_remove(const OrderingVTable *ord, Vec *prev_tree_ptrs, const void *key_view, AvlTreeKV *start_tree);
size_t AvlNodeKV_left_height(const AvlNodeKV *self);
size_t AvlNodeKV_right_height(const AvlNodeKV *self);
int8_t AvlNodeKV_balance_factor(const AvlNodeKV *self);
void AvlNodeKV_update_height(AvlNodeKV *self);
bool AvlNodeKV_rebalance(AvlNodeKV *self);
bool AvlNodeKV_rotate_right(AvlNodeKV *self);
bool AvlNodeKV_rotate_left(AvlNodeKV *self);

struct __AvlTreeKV_Struct {
    // Format: Option<AvlNode<ObjInternal<K>, ObjInternal<V>>>
    Option *avl_node;
};

struct __AvlNodeKV_Struct {
    size_t height;
    AvlTreeKV *left;
    AvlTreeKV *right;
    ObjectInternal key;
    ObjectInternal value;
};

struct __AvlTreeMap_Struct {
    AvlTreeKV *root;    
    const OrderingVTable *ord;
    size_t size;
};

typedef struct __AvlTreeMapIter_Struct {
    // Format: Vec<AvlNode<ObjInternal<K>, ObjInternal<V>> *>
    Vec *prev_nodes;
    AvlTreeKV *curr_tree;
    MapViewKV curr_view;
    AvlTreeMap *tree_map;
} AvlTreeMapIter;

AvlTreeKV *AvlTreeKV_new() {
    AvlTreeKV *tree = malloc(sizeof *tree);
    tree->avl_node = Option_of(NULL);
    return tree;
}

AvlNodeKV *AvlNodeKV_new(ObjectInternal key, ObjectInternal value) {
    AvlNodeKV *node = malloc(sizeof *node);
    node->left = AvlTreeKV_new();
    node->right = AvlTreeKV_new();
    node->height = 1;
    node->key = key;
    node->value = value;
    return node;
}

// ---- AvlTreeMap ---- //

AvlTreeMap *AvlTreeMap_new(const OrderingVTable *table) {
    AvlTreeMap *map = malloc(sizeof *map);
    map->root = AvlTreeKV_new();
    map->ord = table;
    map->size = 0;

    return map;
}

size_t AvlTreeMap_size(const AvlTreeMap *self) {
    return self->size;
}

void _delete_nodeKV(AvlNodeKV *node) {
    if (node == NULL) {
        return;
    }
    if (node->left) {
        _delete_treeKV(node->left);
    }
    if (node->right) {
        _delete_treeKV(node->right);
    }
    free(node);
}

void _delete_treeKV(AvlTreeKV *tree) {
    if (tree == NULL) {
        return;
    }
    if (tree->avl_node == NULL) {
        free(tree);
        return;
    }
    if (Option_is_some(tree->avl_node)) {
        _delete_nodeKV(Option_get(tree->avl_node));
    }
    else {
        Option_delete(tree->avl_node);
    }
    free(tree);
}

void AvlTreeMap_delete(AvlTreeMap **self) {
    if (*self) {
        _delete_treeKV((*self)->root);
        free(*self);
    }
}

bool AvlTreeMap_insert(AvlTreeMap *self, ObjWrap key, ObjWrap value) {
    // Format: Vec<ObjectInternal<AvlNode<ObjectInternal<K>, ObjectInternal<V>> *>>
    Vec *prev_node_ptrs = Vec_new();
    AvlTreeKV *curr_tree = self->root;

    ObjectInternal key_internal = ObjWrap_to_internal(key);
    ObjectInternal value_internal = ObjWrap_to_internal(value);
    
    void *key_view = ObjectInternal_to_view(&key_internal);

    while (Option_is_some(curr_tree->avl_node)) {
        // Get view of the current node
        AvlNodeKV *curr_node = Option_get(Option_clone(curr_tree->avl_node));

        // Push node onto the stack
        Vec_push(prev_node_ptrs, Obj_from_ptr(curr_node));

        // Grab key from current node 
        void *node_key_view = ObjectInternal_to_view(&curr_node->key);

        // Compare key to the curr_node's key
        Ordering ord = self->ord->cmp(node_key_view, key_view);
        switch (ord) {
            case Less:
                // Move curr tree to the right branch
                curr_tree = curr_node->right;
                break;
            case Greater:
                // Move curr tree to the left branch
                curr_tree = curr_node->left;
                break;
            case Equal:
            default:
                // Can't replace existing key/value
                Vec_delete(&prev_node_ptrs);
                return false;
        }
    }
    AvlNodeKV *to_insert = AvlNodeKV_new(key_internal, value_internal);
    bool inserted = Option_insert(curr_tree->avl_node, to_insert);

    if (inserted) {
        Iter_for_each_obj(Vec_iter_reverse(prev_node_ptrs, true), ^ void (ObjWrap item) {
            AvlNodeKV *node = Obj_to_ptr(item);
            AvlNodeKV_update_height(node);
            AvlNodeKV_rebalance(node);
        });
        self->size++;
        return true;
    }
    else {
        Vec_delete(&prev_node_ptrs);
        _delete_nodeKV(to_insert);
        return false;
    }
}

Option_obj *AvlTreeMap_view_obj(const AvlTreeMap *self, ObjWrap key) {
    AvlTreeKV *curr_tree = self->root;

    ObjectInternal key_internal = ObjWrap_to_internal(key);
    const void *key_view = ObjectInternal_to_view(&key_internal);

    Option_obj *ret = Option_obj_empty();
    
    while (!Option_obj_is_some(ret) && Option_is_some(curr_tree->avl_node)) {
        // Get a ref to the current node
        const AvlNodeKV *curr_node = Option_get(Option_clone(curr_tree->avl_node));

        // Grab key view from current node
        const void *node_key_view = ObjectInternal_to_view(&curr_node->key);

        // Compare data to the curr_node's data
        const Ordering ord = self->ord->cmp(node_key_view, key_view);
        switch (ord) {
            case Less:
                // Move curr tree to the right branch
                curr_tree = curr_node->right;
                break;
            case Greater:
                // Move curr tree to the left branch
                curr_tree = curr_node->left;
                break;
            case Equal:
                // Found item
                Option_obj_insert(ret, ObjectInternal_to_wrap(curr_node->value));
        }
    }

    return ret;
}

Option_obj *AvlTreeMap_replace_obj(AvlTreeMap *self, ObjWrap key, ObjWrap value) {
    AvlTreeKV *curr_tree = self->root;

    ObjectInternal value_internal = ObjWrap_to_internal(value);
    ObjectInternal key_internal = ObjWrap_to_internal(key);
    const void *key_view = ObjectInternal_to_view(&key_internal);

    Option_obj *ret = Option_obj_empty();
    
    while (!Option_obj_is_some(ret) && Option_is_some(curr_tree->avl_node)) {
        // Get a ref to the current node
        AvlNodeKV *curr_node = Option_get(Option_clone(curr_tree->avl_node));

        // Grab key view from current node
        const void *node_key_view = ObjectInternal_to_view(&curr_node->key);

        // Compare data to the curr_node's data
        const Ordering ord = self->ord->cmp(node_key_view, key_view);
        switch (ord) {
            case Less:
                // Move curr tree to the right branch
                curr_tree = curr_node->right;
                break;
            case Greater:
                // Move curr tree to the left branch
                curr_tree = curr_node->left;
                break;
            case Equal:
                // Found item
                Option_obj_insert(ret, ObjectInternal_to_wrap(curr_node->value));
                curr_node->value = value_internal;
        }
    }

    return ret;
}

bool AvlTreeMap_is_empty(const AvlTreeMap *self) {
    return !Option_is_some(self->root->avl_node);
}

bool AvlTreeMap_contains_key(const AvlTreeMap *self, const ObjWrap key) {
    AvlTreeKV *curr_tree = self->root;

    ObjectInternal key_internal = ObjWrap_to_internal(key);
    const void *key_view = ObjectInternal_to_view(&key_internal);
    
    while (Option_is_some(curr_tree->avl_node)) {
        // Get a ref to the current node
        const AvlNodeKV *curr_node = Option_get(Option_clone(curr_tree->avl_node));

        // Grab key view from current node
        const void *node_key_view = ObjectInternal_to_view(&curr_node->key);

        // Compare data to the curr_node's data
        const Ordering ord = self->ord->cmp(node_key_view, key_view);
        switch (ord) {
            case Less:
                // Move curr tree to the right branch
                curr_tree = curr_node->right;
                break;
            case Greater:
                // Move curr tree to the left branch
                curr_tree = curr_node->left;
                break;
            case Equal:
                // Found item
                return true;
        }
    }

    // Item not found
    return false;
}

Option_obj *_remove_node_from_tree(Vec *prev_tree_ptrs, Option *target_tree_opt, uint8_t *size_decreaser) {
    Option_obj *return_data = Option_obj_empty();
    if (Option_is_some(target_tree_opt)) {
        // Format: Option<ObjWrap<V>>
        AvlTreeKV *target_tree = Option_get(target_tree_opt);
        AvlNodeKV *target_node = Option_get(Option_clone(target_tree->avl_node));
        if (!(Option_is_some(target_node->left->avl_node) || Option_is_some(target_node->right->avl_node))) {
            // Deleting a child node -- Best scenario, I love this one
            Option_delete(Option_take(target_tree->avl_node)); // We already have ref to AvlNode, no need to save result here
            ObjWrap value = ObjectInternal_to_wrap(target_node->value);
            Option_obj_insert(return_data, value);
            _delete_nodeKV(target_node);
        }
        else if (!Option_is_some(target_node->left->avl_node)) {
            // Deleting a node with right child
            AvlNodeKV *right_node = Option_get(Option_take(target_node->right->avl_node));
            Option_delete(Option_replace(target_tree->avl_node, right_node));
            ObjWrap value = ObjectInternal_to_wrap(target_node->value);
            Option_obj_insert(return_data, value);
            _delete_nodeKV(target_node);
        }
        else if (!Option_is_some(target_node->right->avl_node)) {
            // Deleting a node with left child
            AvlNodeKV *left_node = Option_get(Option_take(target_node->left->avl_node));
            Option_delete(Option_replace(target_tree->avl_node, left_node));
            ObjWrap value = ObjectInternal_to_wrap(target_node->value);
            Option_obj_insert(return_data, value);
            _delete_nodeKV(target_node);
        }
        else {
            // Deleting a node with both children -- Hardest one to code
            // Find largest node in the left subtree
            
            // Format: Vec<AvlTree<Option<AvlNode<ObjectInternal<K>, ObjectInternal<V>> *>> *>
            Vec *target_to_largest_left = Vec_with_capacity(target_node->height); 
            AvlTreeKV *temp_tree = target_node->left;
            while (Option_is_some(temp_tree->avl_node)) {
                Vec_push(target_to_largest_left, Obj_from_ptr(temp_tree));
                AvlNodeKV *temp_node = Option_get(Option_clone(temp_tree->avl_node));
                temp_tree = temp_node->right;
            } // Largest node is at the top of the vec stack

            // temp_tree should be set to a null node, but we can't delete because it's
            // properly attached to another node, so we can just ignore temp_tree.
            AvlTreeKV *largest_tree_before_target = Obj_to_ptr(Option_obj_get(Vec_pop(target_to_largest_left)));
            AvlNodeKV *largest_node_before_target = Option_get(Option_clone(largest_tree_before_target->avl_node));

            // Can't have a right node, since that was the condition for stopping the earlier loop.
            // Can't have both nodes, since we can't have the right node.
            // Therefore, we can only have a left node, or no nodes.
            if (!(Option_is_some(largest_node_before_target->left->avl_node) || Option_is_some(largest_node_before_target->right->avl_node))) {
                // Replace the data only, to prevent having to reconnect the node leaves.
                
                ObjectInternal key_internal = largest_node_before_target->key;
                ObjectInternal value_internal = largest_node_before_target->value;
                ObjWrap value = ObjectInternal_to_wrap(target_node->value);

                // swap 
                target_node->key = key_internal;
                target_node->value = value_internal;
                Option_obj_insert(return_data, value);

                Option_delete(Option_take(largest_tree_before_target->avl_node));
                _delete_nodeKV(largest_node_before_target);
            }
            else {                
                // Still replace the data
                ObjectInternal key_internal = largest_node_before_target->key;
                ObjectInternal value_internal = largest_node_before_target->value;
                ObjWrap value = ObjectInternal_to_wrap(target_node->value);

                // swap 
                target_node->key = key_internal;
                target_node->value = value_internal;
                Option_obj_insert(return_data, value);

                // Now replace the node we just use to replace the data, and replace that with its left node
                AvlNodeKV *left_node = Option_get(Option_clone(largest_node_before_target->left->avl_node));
                Option_delete(Option_replace(largest_tree_before_target->avl_node, left_node));
                _delete_nodeKV(largest_node_before_target);
            }

            // Update node heights, rebalance
            Iter_for_each_obj(Vec_iter_reverse(target_to_largest_left, true), ^ void (ObjWrap tree_ptr) {
                AvlTreeKV *tree = Obj_to_ptr(tree_ptr);
                AvlNodeKV *node = Option_get(Option_clone(tree->avl_node));
                AvlNodeKV_update_height(node);
                AvlNodeKV_rebalance(node);
            });
        }
        Iter_for_each_obj(Vec_iter_reverse(prev_tree_ptrs, false), ^ void (ObjWrap tree_ptr) {
            AvlTreeKV *tree = Obj_to_ptr(tree_ptr);
            AvlNodeKV *node = Option_get(Option_clone(tree->avl_node));
            AvlNodeKV_update_height(node);
            AvlNodeKV_rebalance(node);
        });
        *size_decreaser = 1;
    }
    return return_data;
}

Option *_get_node_to_remove(const OrderingVTable *ord, Vec *prev_tree_ptrs, const void *key_view, AvlTreeKV *start_tree) {
    Option *target_tree = Option_of(NULL);
    AvlTreeKV *curr_tree = start_tree;
    while (!Option_is_some(target_tree) && Option_is_some(curr_tree->avl_node)) {
        AvlNodeKV *curr_node = Option_get(Option_clone(curr_tree->avl_node));
        const void *curr_key_view = ObjectInternal_to_view(&curr_node->key);

        const Ordering ord_val = ord->cmp(curr_key_view, key_view);
        switch (ord_val) {
            case Less:
                Vec_push(prev_tree_ptrs, Obj_from_ptr(curr_tree));
                curr_tree = curr_node->right;
                break;
            case Greater:
                Vec_push(prev_tree_ptrs, Obj_from_ptr(curr_tree));
                curr_tree = curr_node->left;
                break;
            case Equal:
                Option_insert(target_tree, curr_tree);
                break;
        }
    }
    return target_tree;
}

Option_obj *AvlTreeMap_remove_obj(AvlTreeMap *self, const ObjWrap key) {
    // Format: Vec<AvlTree<Option<AvlNode<ObjectInternal<K>, ObjectInternal<V>> *>> *>
    Vec *prev_tree_ptrs = Vec_new(); 

    const ObjectInternal key_internal = ObjWrap_to_internal(key);
    const void *key_view = ObjectInternal_to_view(&key_internal);

    // Format: Option<AvlTree<Option<AvlNode<ObjectInternal<K>, ObjectInternal<V>> *>> *>
    Option *target_tree = _get_node_to_remove(self->ord, prev_tree_ptrs, key_view, self->root);
    
    // Format: Option<ObjWrap<V>>
    uint8_t size_decreaser = 0;
    Option_obj *removed_item = _remove_node_from_tree(prev_tree_ptrs, target_tree, &size_decreaser);
    Vec_delete(&prev_tree_ptrs);
    self->size -= size_decreaser;

    return removed_item;
}

void AvlTreeMap_deep_delete(
        AvlTreeMap *self, 
        void (^key_delete_func)(void *key), 
        void (^value_delete_func)(void *value)
) {
    if (value_delete_func != NULL) {
        Vec *values = Iter_collect(AvlTreeMap_iter_values(self, false), Vec_constr_with_capacity(AvlTreeMap_size(self)));
        Vec_deep_delete(values, value_delete_func);    
    }
    
    if (key_delete_func != NULL) {
        Vec *keys = Iter_collect(AvlTreeMap_iter_keys(self, false), Vec_constr_with_capacity(AvlTreeMap_size(self)));
        Vec_deep_delete(keys, key_delete_func);
    }

    AvlTreeMap_delete(&self);
}

// ---- AvlNodeKV ---- //

bool AvlNodeKV_rebalance(AvlNodeKV *self) {
    switch (AvlNodeKV_balance_factor(self)) {
        case -2: {
            AvlNodeKV *right = Option_get(Option_clone(self->right->avl_node));
            if (AvlNodeKV_balance_factor(right) == 1) {
                AvlNodeKV_rotate_right(right);
            }
            AvlNodeKV_rotate_left(self);
            return true;
        }
        case 2: {
            AvlNodeKV *left = Option_get(Option_clone(self->left->avl_node));
            if (AvlNodeKV_balance_factor(left) == -1) {
                AvlNodeKV_rotate_left(left);
            }
            AvlNodeKV_rotate_right(self);
            return true;
        }
        default: {
            return false;
        }
    }
}

size_t AvlNodeKV_left_height(const AvlNodeKV *self) {
    return Option_map_or_size(Option_clone(self->left->avl_node), 0, ^ size_t (void *left_node) { 
        return ((AvlNodeKV *) left_node)->height;
    });
}

size_t AvlNodeKV_right_height(const AvlNodeKV *self) {
    return Option_map_or_size(Option_clone(self->right->avl_node), 0, ^ size_t (void *right_node) {
        return ((AvlNodeKV *) right_node)->height;
    });
}

int8_t AvlNodeKV_balance_factor(const AvlNodeKV *self) {
    size_t left_height = AvlNodeKV_left_height(self);
    size_t right_height = AvlNodeKV_right_height(self);

    if (left_height >= right_height) {
        return (left_height - right_height);
    }
    else {
        return -(right_height - left_height);
    }
}

void AvlNodeKV_update_height(AvlNodeKV *self) {
    self->height = 1 + max_size(AvlNodeKV_left_height(self), AvlNodeKV_right_height(self));
}

bool AvlNodeKV_rotate_right(AvlNodeKV *self)
{
    if (!Option_is_some(self->left->avl_node)) {
        return false;
    }

    // Grab nodes
    AvlNodeKV *d_node = self; // To not drive me crazy later
    AvlNodeKV *b_node = Option_get(Option_clone(d_node->left->avl_node));
    Option *e_node_opt = d_node->right->avl_node; // Might not exist (probably doesn't, now that I think about it)
    AvlNodeKV *a_node = Option_get(Option_clone(b_node->left->avl_node));
    Option *c_node_opt = b_node->right->avl_node; // Might not exist

    // Swap data from B and D
    ObjectInternal b_node_key = b_node->key;
    ObjectInternal b_node_val = b_node->value;
    b_node->key = d_node->key;
    b_node->value = d_node->value;
    d_node->key = b_node_key;
    d_node->value = b_node_val;

    // Perform re-linking (rotation)
    Option_delete(Option_replace(d_node->left->avl_node, a_node)); // Replace creates an option object, 
                                                                   // so we need to delete it to not leak
    if (Option_is_some(e_node_opt)) {
        AvlNodeKV *e_node = Option_get(Option_clone(e_node_opt));
        Option_delete(Option_replace(b_node->right->avl_node, e_node));
    }
    else {
        Option_delete(Option_take(b_node->right->avl_node));
    } 
    if (Option_is_some(c_node_opt)) {
        AvlNodeKV *c_node = Option_get(Option_clone(c_node_opt));
        Option_delete(Option_replace(b_node->left->avl_node, c_node));
    }
    else {
        Option_delete(Option_take(b_node->left->avl_node));
    }
    Option_delete(Option_replace(d_node->right->avl_node, b_node));

    AvlNodeKV_update_height(b_node);
    AvlNodeKV_update_height(d_node);

    return true;
}

bool AvlNodeKV_rotate_left(AvlNodeKV *self) {
    if (!Option_is_some(self->right->avl_node)) {
        return false;
    }

    // Grab nodes
    AvlNodeKV *d_node = self; // To not drive me crazy later
    AvlNodeKV *b_node = Option_get(Option_clone(d_node->right->avl_node));
    Option *e_node_opt = d_node->left->avl_node; // Might not exist (probably doesn't, now that I think about it)
    AvlNodeKV *a_node = Option_get(Option_clone(b_node->right->avl_node));
    Option *c_node_opt = b_node->left->avl_node; // Might not exist

    // Swap data from B and D
    ObjectInternal b_node_key = b_node->key;
    ObjectInternal b_node_val = b_node->value;
    b_node->key = d_node->key;
    b_node->value = d_node->value;
    d_node->key = b_node_key;
    d_node->value = b_node_val;


    // Perform re-linking (rotation)
    Option_delete(Option_replace(d_node->right->avl_node, a_node)); // Replace creates an option object, 
                                                                    // so we need to delete it to not leak
    if (Option_is_some(e_node_opt)) {
        AvlNodeKV *e_node = Option_get(Option_clone(e_node_opt));
        Option_delete(Option_replace(b_node->left->avl_node, e_node));
    }
    else {
        Option_delete(Option_take(b_node->left->avl_node));
    }
    if (Option_is_some(c_node_opt)) {
        AvlNodeKV *c_node = Option_get(Option_clone(c_node_opt));
        Option_delete(Option_replace(b_node->right->avl_node, c_node));
    }
    else {
        Option_delete(Option_take(b_node->right->avl_node));
    }
    Option_delete(Option_replace(d_node->left->avl_node, b_node));

    AvlNodeKV_update_height(b_node);
    AvlNodeKV_update_height(d_node);

    return true;
}


// ---- Iterator ---- //

void *AvlTreeMapIter_from(AvlTreeMap *self) {
    AvlTreeMapIter *iter = malloc(sizeof *iter);
    iter->curr_tree = self->root;
    iter->prev_nodes = Vec_new();
    iter->tree_map = self;
    return iter;
}

void AvlTreeMapIter_delete(void *self_as_void_ptr, bool delete_collection) {
    AvlTreeMapIter *self = self_as_void_ptr;
    Vec_delete(&self->prev_nodes);
    if (delete_collection) {
        AvlTreeMap_delete(&self->tree_map);
    }
    free(self);
}

Option_obj *AvlTreeMapIter_next(void *self_as_void_ptr) {
    AvlTreeMapIter *self = self_as_void_ptr;
    while (Option_is_some(self->curr_tree->avl_node)) {
        Option *as_ref = Option_clone(self->curr_tree->avl_node);
        AvlNodeKV *curr_node = Option_get(as_ref);
        Vec_push(self->prev_nodes, Obj_from_ptr(curr_node));
        self->curr_tree = curr_node->left;
    }

    if (!Vec_empty(self->prev_nodes)) {
        Option_obj *pop = Vec_pop(self->prev_nodes);
        AvlNodeKV *popped_node = Obj_to_ptr(Option_obj_get(pop));
        self->curr_tree = popped_node->right;
        self->curr_view.key = ObjectInternal_to_wrap(popped_node->key);
        self->curr_view.value = ObjectInternal_to_wrap(popped_node->value);
        // printf("Right before leaving AvlTreeSetIter_next: data = %p\n", popped_node->data);
        return Option_obj_of(Obj_from_ptr(&self->curr_view));
    }
    else {
        return Option_obj_empty();
    }
}

static const struct __Obj_Iterator_VTable_Struct AvlTreeMap_obj_iter_vtable = {
    .next = ^ Option_obj *(void *self_as_void_ptr) {
        return AvlTreeMapIter_next(self_as_void_ptr);
    },
    .delete_iterable_obj = ^ void (void *self_as_void_ptr, bool delete_collection) {
        AvlTreeMapIter_delete(self_as_void_ptr, delete_collection);
    }
};

Iterator AvlTreeMap_iter(AvlTreeMap *self, bool delete_collection_after_iter) {
    return (Iterator) {
        .iterable_obj = AvlTreeMapIter_from(self),
        .delete_collection_after_iter = delete_collection_after_iter,
        .vtable = AvlTreeMap_obj_iter_vtable
    };
}

Iterator AvlTreeMap_iter_keys(AvlTreeMap *self, bool delete_collection_after_iter) {
    return (Iterator) {
        .iterable_obj = AvlTreeMapIter_from(self),
        .delete_collection_after_iter = delete_collection_after_iter,
        .vtable = (IteratorVTable) {
            .next = ^ Option_obj *(void *self_as_void_ptr) {
                Option_obj *next_item = AvlTreeMapIter_next(self_as_void_ptr);
                if (Option_obj_is_some(next_item)) {
                    MapViewKV *view = Obj_to_ptr(Option_obj_get(Option_obj_take(next_item)));
                    Option_obj_insert(next_item, view->key);
                } 
                return next_item;
            },
            .delete_iterable_obj = ^ void (void *self_as_void_ptr, bool delete_collection) {
                AvlTreeMapIter_delete(self_as_void_ptr, delete_collection);
            }
        }    
    };
}

Iterator AvlTreeMap_iter_values(AvlTreeMap *self, bool delete_collection_after_iter) {
    return (Iterator) {
        .iterable_obj = AvlTreeMapIter_from(self),
        .delete_collection_after_iter = delete_collection_after_iter,
        .vtable = (IteratorVTable) {
            .next = ^ Option_obj *(void *self_as_void_ptr) {
                Option_obj *next_item = AvlTreeMapIter_next(self_as_void_ptr);
                if (Option_obj_is_some(next_item)) {
                    MapViewKV *view = Obj_to_ptr(Option_obj_get(Option_obj_take(next_item)));
                    Option_obj_insert(next_item, view->value);
                } 
                return next_item;
            },
            .delete_iterable_obj = ^ void (void *self_as_void_ptr, bool delete_collection) {
                AvlTreeMapIter_delete(self_as_void_ptr, delete_collection);
            }
        }    
    };
}



// ---- Map Wrapper Functions ---- //

Iterator _avl_map_iter(void *map, bool delete_collection_after_iter) { return AvlTreeMap_iter(map, delete_collection_after_iter); }
Iterator _avl_map_iter_keys(void *map, bool delete_collection_after_iter) { return AvlTreeMap_iter_keys(map, delete_collection_after_iter); }
Iterator _avl_map_iter_values(void *map, bool delete_collection_after_iter) { return AvlTreeMap_iter_values(map, delete_collection_after_iter); }
bool _avl_map_push(void *map, ObjWrap key, ObjWrap value) { return AvlTreeMap_insert(map, key, value); }
Option_obj *_avl_map_view_obj(const void *map, ObjWrap key) { return AvlTreeMap_view_obj(map, key); }
Option_obj *_avl_map_remove_obj(void *map, ObjWrap key) { return AvlTreeMap_remove_obj(map, key); }
Option_obj *_avl_map_replace_obj(void *map, ObjWrap key, ObjWrap value) { return AvlTreeMap_replace_obj(map, key, value); }
bool _avl_map_empty(const void *map) { return AvlTreeMap_is_empty(map); }
bool _avl_map_contains_key(const void *map, ObjWrap key) { return AvlTreeMap_contains_key(map, key); }
void _avl_map_delete_map(void *map) { return AvlTreeMap_delete((AvlTreeMap **) &map); }
void _avl_map_deep_delete(void *map, void (^free_key_func)(void *key), void (^free_value_func)(void *value)) { AvlTreeMap_deep_delete(map, free_key_func, free_value_func); }

static const MapVTable AvlTreeMap_map_vtable = {
    .contains_key = _avl_map_contains_key,
    .delete_map = _avl_map_delete_map,
    .empty = _avl_map_empty,
    .iter = _avl_map_iter,
    .iter_keys = _avl_map_iter_keys,
    .iter_values = _avl_map_iter_values,
    .push = _avl_map_push,
    .remove_obj = _avl_map_remove_obj,
    .replace_obj = _avl_map_replace_obj,
    .view_obj = _avl_map_view_obj,
};

Map AvlTreeMap_as_map(AvlTreeMap *self) {
    return (Map) {
        .map = self,
        .vtable = &AvlTreeMap_map_vtable,
    };
}

