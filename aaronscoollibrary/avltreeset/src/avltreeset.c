#include "avltreeset.h"
#include "interface/interface_i32.h"
#include "interface/interface_str.h"
#include "interface/iterator.h"
#include "vector.h"
#include "munit.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>


// ---- Utils ---- //
size_t max_size(size_t a, size_t b) { return (a > b) ? a : b; }
uint64_t max_u64(uint64_t a, uint64_t b) { return (a > b) ? a : b; }
void *_replace_ptr(void **dest, void *src) 
{ 
    void *ret = *dest;
    *dest = src;
    return ret;
}
void _swap_ptrs(void **ptr1, void **ptr2) 
{ 
    void *tmp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = tmp;
}


AvlTree *AvlTree_new();
AvlNode *AvlNode_new(void *data);
void _delete_tree(AvlTree *tree);
void _delete_node(AvlNode *node);
size_t AvlNode_left_height(const AvlNode *self);
size_t AvlNode_right_height(const AvlNode *self);
int8_t AvlNode_balance_factor(const AvlNode *self);
void AvlNode_update_height(AvlNode *self);
bool AvlNode_rebalance(AvlNode *self);
bool AvlNode_rotate_right(AvlNode *self);
bool AvlNode_rotate_left(AvlNode *self);
 
struct __AvlNode_Struct
{
    // 'data' is a pointer to memory.
    // Item *item = /* malloc'd memory */;
    // AvlTreeSet_insert(/* treeset */,  &item  );
    // AvlTreeSet does NOT hold a copy of the data, only a ptr to it
    void *data;
    size_t height;
    AvlTree *left;
    AvlTree *right;
};

struct __AvlTree_Struct
{
    // Format: Option<*AvlNode<T>>
    Option *avl_node;
};


struct __AvlTreeSet_Struct
{
    AvlTree *root;
    const OrderingVTable *table;
};

typedef struct __AvlTreeSetIter_Struct
{
    // Format: Vec<*AvlNode<T>>
    Vec *prev_nodes;
    AvlTree *curr_tree;
    AvlTreeSet *tree_set;
} AvlTreeSetIter;

AvlTree *AvlTree_new()
{
    AvlTree *tree = malloc(sizeof *tree);
    tree->avl_node = Option_of(NULL);
    return tree;
}


AvlNode *AvlNode_new(void *data)
{
    AvlNode *node = malloc(sizeof *node);
    node->data = data;
    node->left = AvlTree_new();
    node->right = AvlTree_new();
    node->height = 1;
    return node;
}

// ---- AvlTreeSet ---- //

AvlTreeSet *AvlTreeSet_new()
{
    return AvlTreeSet_with_ordering(&ordering_str);
}

AvlTreeSet *AvlTreeSet_with_ordering(const OrderingVTable *table)
{
    AvlTreeSet *set = malloc(sizeof *set);
    set->root = AvlTree_new();
    set->table = table;

    return set;
}

void _delete_node(AvlNode *node)
{
    if (node == NULL)
    {
        return;
    }
    if (node->left)
    {
        _delete_tree(node->left);
    }
    if (node->right)
    {
        _delete_tree(node->right);
    }
    free(node);
}

void _delete_tree(AvlTree *tree)
{
    if (tree == NULL)
    {
        return;
    }
    if (tree->avl_node == NULL)
    {
        free(tree);
        return;
    }
    if (Option_is_some(tree->avl_node))
    {
        _delete_node(Option_get(tree->avl_node));
    }
    else 
    {
        Option_delete(tree->avl_node);
    }
    free(tree);
}

void AvlTreeSet_delete(AvlTreeSet **self_ref)
{
    if (*self_ref)
    {
        _delete_tree((*self_ref)->root);
        free(*self_ref);
    }
}

bool AvlTreeSet_insert(const AvlTreeSet *self, void *item)
{
    Vec *prev_node_ptrs = Vec_new();
    AvlTree *curr_tree = self->root;
    
    while (Option_is_some(curr_tree->avl_node))
    {
        // Take current node out of option to get a ref to it,
        // then put back into the option
        AvlNode *curr_node = Option_get(Option_clone(curr_tree->avl_node));

        // Push node onto the stack
        Vec_push(prev_node_ptrs, curr_node);

        // Compare data to the curr_node's data
        Ordering ord = self->table->cmp(curr_node->data, item);
        switch (ord)
        {
            case Less:
            {
                // Move curr tree to the right branch
                curr_tree = curr_node->right;
                break;
            }
            case Greater:
            {
                // Move curr tree to the left branch
                curr_tree = curr_node->left;
                break;
            }
            case Equal:
            default:
            {
                // Two items can't be the same
                Vec_delete(prev_node_ptrs);
                return false;
            }
        }
    }
    AvlNode *to_insert = AvlNode_new(item);
    bool inserted = Option_insert(curr_tree->avl_node, to_insert);

    if (inserted)
    {
        Iter_for_each(Vec_iter_reverse(prev_node_ptrs, true), ^ void (void *item) 
        {
            AvlNode *node = item;
            AvlNode_update_height(node);
            AvlNode_rebalance(node);
        });
        return true;
    }
    else
    {
        Vec_delete(prev_node_ptrs);
        _delete_node(to_insert);
        return false;
    }
}

bool AvlTreeSet_is_empty(const AvlTreeSet *self)
{
    return Option_is_some(self->root->avl_node);
}

bool AvlTreeSet_contains(const AvlTreeSet *self, const void *item)
{
    AvlTree *curr_tree = self->root;
    
    while (Option_is_some(curr_tree->avl_node))
    {
        // Get a ref to the current node
        const AvlNode *curr_node = Option_get(Option_clone(curr_tree->avl_node));

        // Compare data to the curr_node's data
        const Ordering ord = self->table->cmp(curr_node->data, item);
        switch (ord)
        {
            case Less:
            {
                // Move curr tree to the right branch
                curr_tree = curr_node->right;
                break;
            }
            case Greater:
            {
                // Move curr tree to the left branch
                curr_tree = curr_node->left;
                break;
            }
            case Equal:
            default:
            {
                // Found item
                return true;
            }
        }
    }

    // Item not found
    return false;
}

Option *_remove_node_from_tree(const AvlTreeSet *self, Vec *prev_node_ptrs, Option *target_val)
{
    Option *__RETURN_VALUE__ = Option_of(NULL);
    // if (Option_is_some(target_val))
    // {
    //     AvlNode *target_node = Option_get(target_val);
    //
    //     // Delete that node
    //     if (!Option_is_some(target_node->left->avl_node) 
    //             || !Option_is_some(target_node->right->avl_node))
    //     {
    //         if (Option_is_some(target_node->left->avl_node))
    //         {
    //             AvlNode *left_node = Option_take(target_node->left->avl_node);
    //             AvlNode *to_delete = _replace_ptr((void **) &target_node, left_node); // Probably going to cause problems (IT DID)
    //             Option_insert(__RETURN_VALUE__, to_delete->data);
    //             _delete_node(to_delete);
    //         }
    //         else if (Option_is_some(target_node->right->avl_node))
    //         {
    //             AvlNode *right_node = Option_take(target_node->right->avl_node);
    //             AvlNode *to_delete = _replace_ptr((void **) &target_node, right_node); // Probably going to cause problems (SO DID THIS ONE) (PROBABLY)
    //             Option_insert(__RETURN_VALUE__, to_delete->data);
    //             _delete_node(to_delete);
    //         }
    //         else
    //         {
    //             // Deleting with no children
    //             Option *popped_node = Vec_pop(prev_node_ptrs);
    //             if (Option_is_some(popped_node))
    //             {
    //                 AvlNode *parent_node = Option_get(popped_node);
    //                 if (Option_is_some(parent_node->left->avl_node))
    //                 {
    //                     AvlNode *left_node_ref = Option_get(Option_as_ref(parent_node->left->avl_node));
    //                     const Ordering ord = self->table->cmp(left_node_ref->data, target_node->data);
    //                     if (ord == Equal)
    //                     {
    //                         AvlNode *to_delete = Option_take(parent_node->left->avl_node);
    //                         Option_insert(__RETURN_VALUE__, to_delete->data);
    //                         _delete_node(to_delete);
    //
    //                     }
    //                     else
    //                     {
    //                         AvlNode *to_delete = Option_take(parent_node->right->avl_node);
    //                         Option_insert(__RETURN_VALUE__, to_delete->data);
    //                         _delete_node(to_delete);
    //                     }
    //                 }
    //                 else
    //                 {
    //                     AvlNode *to_delete = Option_take(parent_node->right->avl_node);
    //                     Option_insert(__RETURN_VALUE__, to_delete->data);
    //                     _delete_node(to_delete);
    //                 }
    //
    //                 AvlNode_update_height(parent_node);
    //                 AvlNode_rebalance(parent_node);
    //             }
    //             else
    //             {
    //                 Option_delete(popped_node);
    //                 AvlNode *to_delete = Option_take(self->root->avl_node);
    //                 Option_insert(__RETURN_VALUE__, to_delete->data);
    //                 _delete_node(to_delete);
    //             }
    //         }
    //     }
    //     else
    //     {
    //         // TODO: Two children deletion
    //         AvlTree *right_tree = target_node->right;
    //         Option *right_tree_as_ref_left = ((AvlNode *) Option_get(Option_as_ref(right_tree->avl_node)))->left->avl_node;
    //
    //         if (!Option_is_some(right_tree_as_ref_left))
    //         {
    //             AvlNode *right_node = Option_take(right_tree->avl_node);
    //
    //             void *inner_val = _replace_ptr(&target_node->data, right_node->data);
    //             Option_insert(__RETURN_VALUE__, inner_val);
    //             AvlNode *to_delete = Option_get(Option_replace(target_node->right->avl_node, Option_take(right_node->right->avl_node)));
    //             _delete_node(to_delete);
    //
    //             AvlNode_update_height(target_node);
    //             AvlNode_rebalance(target_node);
    //
    //             return __RETURN_VALUE__;
    //         }
    //         else
    //         {
    //             // 1. Starting with the right child of the target node
    //             AvlTree *next_tree = right_tree;
    //             // We also keep track of the parent nodes to update the height
    //             Vec *inner_ptrs = Vec_new();
    //
    //             // 2. Keep moving the current node to the left child until it has none
    //             while (Option_is_some(next_tree->avl_node))
    //             {
    //                 AvlNode *next_left_node = Option_get(Option_as_ref(next_tree->avl_node));
    //                 if (Option_is_some(next_left_node->left->avl_node))
    //                 {
    //                     Vec_push(inner_ptrs, next_left_node);
    //                 }
    //                 next_tree = next_left_node->left;
    //             }
    //
    //             // We don't use next_tree but instead the tracked nodes as basis
    //             AvlNode *parent_left_node = Option_get(Vec_pop(inner_ptrs));
    //             AvlNode *leftmost_node = Option_take(parent_left_node->left->avl_node);
    //
    //             // 3. Replace the target node value with the current node value
    //             void *inner_val = _replace_ptr(&target_node->data, leftmost_node->data);
    //             Option_insert(__RETURN_VALUE__, inner_val);
    //             // 4. Replace the current node with the right child if it has
    //             AvlNode *to_delete = Option_get(Option_replace(parent_left_node->left->avl_node, 
    //                     Option_take(leftmost_node->right->avl_node)));
    //             _delete_node(to_delete);
    //
    //             AvlNode_update_height(parent_left_node);
    //             AvlNode_rebalance(parent_left_node);
    //
    //             Vec_delete(inner_ptrs);
    //         }
    //     }
    //     
    //     // Retrace the path and update them accordingly
    //     for_each(Vec_iter_reverse(prev_node_ptrs), ^ void (void *item)
    //     {
    //         AvlNode *node = item;
    //         AvlNode_update_height(node);
    //         AvlNode_rebalance(node);
    //     });
    //
    //     AvlNode_update_height(target_node);
    //     AvlNode_rebalance(target_node);
    // }
    // else 
    // {
    //     Option_delete(target_val);
    // }

    return __RETURN_VALUE__;
}

Option *AvlTreeSet_remove(const AvlTreeSet *self, const void *item)
{
    Vec *prev_node_ptrs = Vec_new();
    AvlTree *curr_tree = self->root;
    Option *target_val = Option_of(NULL);

    while (!Option_is_some(target_val) && Option_is_some(curr_tree->avl_node))
    {
        AvlNode *curr_node = Option_get(Option_clone(curr_tree->avl_node));
        const Ordering ord = self->table->cmp(curr_node->data, item);
        switch (ord)
        {
            case Less:
            {
                Vec_push(prev_node_ptrs, curr_node);
                curr_tree = curr_node->right;
                break;
            }
            case Greater:
            {
                Vec_push(prev_node_ptrs, curr_node);
                curr_tree = curr_node->left;
                break;
            }
            case Equal:
            {
                Option_insert(target_val, curr_node);
                break;
            }
        }
    }

    Option *removed_item = _remove_node_from_tree(self, prev_node_ptrs, target_val);
    Vec_delete(prev_node_ptrs);

    return removed_item;
}

void AvlTreeSetConstr_delete(ConstructorVTable *self)
{
    self->collection = NULL;
    free(self);
}

bool AvlTreeSetConstr_push(void *tree_set, void *item)
{
    return AvlTreeSet_insert((AvlTreeSet *) tree_set, item);
}

ConstructorVTable *AvlTreeSet_constr(OrderingVTable *ordering)
{
    AvlTreeSet *tree = AvlTreeSet_with_ordering(ordering);
    ConstructorVTable *constr = malloc(sizeof *constr);
    constr->collection = tree;
    constr->Item_push = AvlTreeSetConstr_push;
    constr->Self_delete = AvlTreeSetConstr_delete;
    return constr;
}

// ---- AvlNode ---- //

bool AvlNode_rebalance(AvlNode *self)
{
    switch (AvlNode_balance_factor(self))
    {
        case -2:
        {
            AvlNode *right = Option_get(Option_clone(self->right->avl_node));
            if (AvlNode_balance_factor(right) == 1)
            {
                AvlNode_rotate_right(right);
            }
            AvlNode_rotate_left(self);
            return true;
        }
        case 2:
        {
            AvlNode *left = Option_get(Option_clone(self->left->avl_node));
            if (AvlNode_balance_factor(left) == -1)
            {
                AvlNode_rotate_left(left);
            }
            AvlNode_rotate_right(self);
            return true;
        }
        default:
        {
            return false;
        }
    }
}

size_t AvlNode_left_height(const AvlNode *self) 
{
    return Option_map_or_size(Option_clone(self->left->avl_node), 0, ^ size_t (void *left_node) { 
        return ((AvlNode *) left_node)->height;
    });
}

size_t AvlNode_right_height(const AvlNode *self)
{
    return Option_map_or_size(Option_clone(self->right->avl_node), 0, ^ size_t (void *right_node) {
        return ((AvlNode *) right_node)->height;
    });
}

int8_t AvlNode_balance_factor(const AvlNode *self)
{
    size_t left_height = AvlNode_left_height(self);
    size_t right_height = AvlNode_right_height(self);

    if (left_height >= right_height)
    {
        return (left_height - right_height);
    }
    else 
    {
        return -(right_height - left_height);
    }
}

void AvlNode_update_height(AvlNode *self)
{
    self->height = 1 + max_size(AvlNode_left_height(self), AvlNode_right_height(self));
}

bool AvlNode_rotate_right(AvlNode *self)
{
    if (!Option_is_some(self->left->avl_node))
    {
        return false;
    }

    // Grab nodes
    AvlNode *d_node = self; // To not drive me crazy later
    AvlNode *b_node = Option_get(Option_clone(d_node->left->avl_node));
    Option *e_node_opt = d_node->right->avl_node; // Might not exist (probably doesn't, now that I think about it)
    AvlNode *a_node = Option_get(Option_clone(b_node->left->avl_node));
    Option *c_node_opt = b_node->right->avl_node; // Might not exist

    // Swap data from B and D
    _swap_ptrs(&b_node->data, &d_node->data);

    // Perform re-linking (rotation)
    Option_delete(Option_replace(d_node->left->avl_node, a_node)); // Replace creates an option object, 
                                                                   // so we need to delete it to not leak
    if (Option_is_some(e_node_opt))
    {
        AvlNode *e_node = Option_get(Option_clone(e_node_opt));
        Option_delete(Option_replace(b_node->right->avl_node, e_node));
    }
    else
    {
        Option_delete(Option_take(b_node->right->avl_node));
    } 
    if (Option_is_some(c_node_opt))
    {
        AvlNode *c_node = Option_get(Option_clone(c_node_opt));
        Option_delete(Option_replace(b_node->left->avl_node, c_node));
    }
    else
    {
        Option_delete(Option_take(b_node->left->avl_node));
    }
    Option_delete(Option_replace(d_node->right->avl_node, b_node));

    AvlNode_update_height(b_node);
    AvlNode_update_height(d_node);

    return true;
}

bool AvlNode_rotate_left(AvlNode *self)
{
    if (!Option_is_some(self->right->avl_node))
    {
        return false;
    }

    // Grab nodes
    AvlNode *d_node = self; // To not drive me crazy later
    AvlNode *b_node = Option_get(Option_clone(d_node->right->avl_node));
    Option *e_node_opt = d_node->left->avl_node; // Might not exist (probably doesn't, now that I think about it)
    AvlNode *a_node = Option_get(Option_clone(b_node->right->avl_node));
    Option *c_node_opt = b_node->left->avl_node; // Might not exist

    // Swap data from B and D
    _swap_ptrs(&b_node->data, &d_node->data);

    // Perform re-linking (rotation)
    Option_delete(Option_replace(d_node->right->avl_node, a_node)); // Replace creates an option object, 
                                                                    // so we need to delete it to not leak
    if (Option_is_some(e_node_opt))
    {
        AvlNode *e_node = Option_get(Option_clone(e_node_opt));
        Option_delete(Option_replace(b_node->left->avl_node, e_node));
    }
    else
    {
        Option_delete(Option_take(b_node->left->avl_node));
    }
    if (Option_is_some(c_node_opt))
    {
        AvlNode *c_node = Option_get(Option_clone(c_node_opt));
        Option_delete(Option_replace(b_node->right->avl_node, c_node));
    }
    else
    {
        Option_delete(Option_take(b_node->right->avl_node));
    }
    Option_delete(Option_replace(d_node->left->avl_node, b_node));

    AvlNode_update_height(b_node);
    AvlNode_update_height(d_node);

    return true;
}


// ---- Iterator ---- //

Option *AvlTreeSetIter_next(void *self_as_void_ptr)
{
    AvlTreeSetIter *self = self_as_void_ptr;
    while (Option_is_some(self->curr_tree->avl_node))
    {
        Option *as_ref = Option_clone(self->curr_tree->avl_node);
        AvlNode *curr_node = Option_get(as_ref);
        Vec_push(self->prev_nodes, curr_node);
        self->curr_tree = curr_node->left;
    }

    if (!Vec_empty(self->prev_nodes))
    {
        Option *pop = Vec_pop(self->prev_nodes);
        AvlNode *popped_node = Option_get(pop);
        self->curr_tree = popped_node->right;
        // printf("Right before leaving AvlTreeSetIter_next: data = %p\n", popped_node->data);
        return Option_of(popped_node);
    }
    else
    {
        return Option_of(NULL);
    }
}

Option *AvlTreeSetIter_next_node_data(void *self_as_void_ptr)
{
    return Option_map(AvlTreeSetIter_next(self_as_void_ptr), 
            ^ Option *(void *node) 
    { 
        AvlNode *avl_node = node;
        // printf("Inside closure of AvlTreeSetIter_next_node_data: data = %p\n", avl_node->data);
        return Option_of(avl_node->data); 
    });
}

void *AvlTreeSetIter_from(AvlTreeSet *self)
{
    AvlTreeSetIter *iter = malloc(sizeof *iter);
    iter->curr_tree = self->root;
    iter->prev_nodes = Vec_new();
    iter->tree_set = self;
    return iter;
}

void AvlTreeSetIter_delete(void *self_as_void_ptr, bool delete_collection)
{
    AvlTreeSetIter *self = self_as_void_ptr;
    Vec_delete(self->prev_nodes);
    if (delete_collection)
    {
        AvlTreeSet_delete(&self->tree_set);
    }
    free(self);
}

IteratorVTable AvlTreeSet_iter(AvlTreeSet *self, bool delete_collection_after_iter)
{
    IteratorVTable table;
    table.iterable_object = AvlTreeSetIter_from(self);
    table.next = ^ Option *(void *self_as_void_ptr) {
        return AvlTreeSetIter_next_node_data(self_as_void_ptr);
    };
    table.delete_iter = AvlTreeSetIter_delete;
    table.delete_collection_after_iter = delete_collection_after_iter;
    return table;
}

IteratorVTable *AvlTreeSet_node_iter(AvlTreeSet *self)
{
    IteratorVTable *table = malloc(sizeof *table);
    table->iterable_object = AvlTreeSetIter_from(self);
    table->next = ^Option *(void *self_as_void_ptr) {
        return AvlTreeSetIter_next(self_as_void_ptr);
    };
    table->delete_iter = AvlTreeSetIter_delete;
    return table;
}


// ---- Set wrapper functions ---- //

bool _contains(const void *set, const void *item) { return AvlTreeSet_contains(set, item); }
IteratorVTable _iter(void *set, bool delete_collection_after_iter) 
{ 
    return AvlTreeSet_iter(set, delete_collection_after_iter); 
}
bool _push(void *set, void *item) { return AvlTreeSet_insert(set, item); }
bool _empty(const void *set) { return AvlTreeSet_is_empty(set); }

SetVTable AvlTreeSet_as_set(AvlTreeSet *self) 
{
    SetVTable set = {
        .set = self,
        .contains = _contains,
        .iter = _iter,
        .push = _push,
        .empty = _empty,
        .ord = self->table,
        .difference_of = NULL,
        .equal = NULL,
        .intersection_of = NULL,
        .pop = NULL,
        .remove = NULL,
        .union_of = NULL,
    };
    return set;
}
