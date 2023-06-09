#include "avltreeset.h"
#include "interface/interface_i32.h"
#include "interface/interface_str.h"
#include "interface/iterator.h"
#include "vector/vector.h"
#include "utils.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>


// ---- Utils ---- //
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

typedef struct __AvlNodeT_Struct AvlNodeT;
typedef struct __AvlTreeT_Struct AvlTreeT;

AvlTreeT *AvlTreeT_new();
AvlNodeT *AvlNodeT_new(void *data);
void _delete_treeT(AvlTreeT *tree);
void _delete_nodeT(AvlNodeT *node);
Option *_remove_nodeT_from_treeT(Vec *prev_node_trees, Option *target_tree_opt);
size_t AvlNodeT_left_height(const AvlNodeT *self);
size_t AvlNodeT_right_height(const AvlNodeT *self);
int8_t AvlNodeT_balance_factor(const AvlNodeT *self);
void AvlNodeT_update_height(AvlNodeT *self);
bool AvlNodeT_rebalance(AvlNodeT *self);
bool AvlNodeT_rotate_right(AvlNodeT *self);
bool AvlNodeT_rotate_left(AvlNodeT *self);
 
struct __AvlNodeT_Struct
{
    // 'data' is a pointer to memory.
    // Item *item = /* malloc'd memory */;
    // AvlTreeSet_insert(/* treeset */,  &item  );
    // AvlTreeSet does NOT hold a copy of the data, only a ptr to it
    void *data;
    size_t height;
    AvlTreeT *left;
    AvlTreeT *right;
};

struct __AvlTreeT_Struct
{
    // Format: Option<*AvlNode<T>>
    Option *avl_node;
};

struct __AvlTreeSet_Struct
{
    AvlTreeT *root;
    const OrderingVTable *table;
};

typedef struct __AvlTreeSetIter_Struct
{
    // Format: Vec<*AvlNode<T>>
    Vec *prev_nodes;
    AvlTreeT *curr_tree;
    AvlTreeSet *tree_set;
} AvlTreeSetIter;

AvlTreeT *AvlTreeT_new()
{
    AvlTreeT *tree = malloc(sizeof *tree);
    tree->avl_node = Option_of(NULL);
    return tree;
}


AvlNodeT *AvlNodeT_new(void *data)
{
    AvlNodeT *node = malloc(sizeof *node);
    node->data = data;
    node->left = AvlTreeT_new();
    node->right = AvlTreeT_new();
    node->height = 1;
    return node;
}

// ---- AvlTreeSet ---- //

AvlTreeSet *AvlTreeSet_new(const OrderingVTable *table)
{
    AvlTreeSet *set = malloc(sizeof *set);
    set->root = AvlTreeT_new();
    set->table = table;

    return set;
}

void _delete_nodeT(AvlNodeT *node)
{
    if (node == NULL)
    {
        return;
    }
    if (node->left)
    {
        _delete_treeT(node->left);
    }
    if (node->right)
    {
        _delete_treeT(node->right);
    }
    free(node);
}

void _delete_treeT(AvlTreeT *tree)
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
        _delete_nodeT(Option_get(tree->avl_node));
    }
    else 
    {
        Option_delete(tree->avl_node);
    }
    free(tree);
}

void AvlTreeSet_delete(AvlTreeSet **self)
{
    if (*self)
    {
        _delete_treeT((*self)->root);
        free(*self);
    }
}

bool AvlTreeSet_insert(const AvlTreeSet *self, void *item)
{
    Vec *prev_node_ptrs = Vec_new();
    AvlTreeT *curr_tree = self->root;
    
    while (Option_is_some(curr_tree->avl_node))
    {
        // Take current node out of option to get a ref to it,
        // then put back into the option
        AvlNodeT *curr_node = Option_get(Option_clone(curr_tree->avl_node));

        // Push node onto the stack
        Vec_push(prev_node_ptrs, Obj_from_ptr(curr_node));

        // Compare data to the curr_node's data
        Ordering ord = self->table->cmp(curr_node->data, item);
        switch (ord)
        {
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
                // Two items can't be the same
                Vec_delete(&prev_node_ptrs);
                return false;
        }
    }
    AvlNodeT *to_insert = AvlNodeT_new(item);
    bool inserted = Option_insert(curr_tree->avl_node, to_insert);

    if (inserted)
    {
        Iter_for_each_obj(Vec_iter_reverse(prev_node_ptrs, true), ^ void (ObjWrap item) 
        {
            AvlNodeT *node = Obj_to_ptr(item);
            AvlNodeT_update_height(node);
            AvlNodeT_rebalance(node);
        });
        return true;
    }
    else
    {
        Vec_delete(&prev_node_ptrs);
        _delete_nodeT(to_insert);
        return false;
    }
}

bool AvlTreeSet_is_empty(const AvlTreeSet *self)
{
    return Option_is_some(self->root->avl_node);
}

bool AvlTreeSet_contains(const AvlTreeSet *self, const void *item)
{
    AvlTreeT *curr_tree = self->root;
    
    while (Option_is_some(curr_tree->avl_node))
    {
        // Get a ref to the current node
        const AvlNodeT *curr_node = Option_get(Option_clone(curr_tree->avl_node));

        // Compare data to the curr_node's data
        const Ordering ord = self->table->cmp(curr_node->data, item);
        switch (ord)
        {
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
                // Found item
                return true;
        }
    }

    // Item not found
    return false;
}

Option *_remove_nodeT_from_treeT(Vec *prev_node_trees, Option *target_tree_opt)
{
    if (Option_is_some(target_tree_opt))
    {
        Option *return_data = Option_of(NULL);
        AvlTreeT *target_tree = Option_get(target_tree_opt);
        AvlNodeT *target_node = Option_get(Option_clone(target_tree->avl_node));
        if (!(Option_is_some(target_node->left->avl_node) || Option_is_some(target_node->right->avl_node)))
        {
            // Deleting a child node -- Best scenario, I love this one
            Option_delete(Option_take(target_tree->avl_node));
            Option_insert(return_data, target_node->data);
            _delete_nodeT(target_node);
        }
        else if (!Option_is_some(target_node->left->avl_node))
        {
            // Deleting a node with right child
            AvlNodeT *right_node = Option_get(Option_take(target_node->right->avl_node));
            Option_delete(Option_replace(target_tree->avl_node, right_node));
            _delete_nodeT(target_node);
        }
        else if (!Option_is_some(target_node->right->avl_node))
        {
            // Deleting a node with left child
            AvlNodeT *left_node = Option_get(Option_take(target_node->left->avl_node));
            Option_delete(Option_replace(target_tree->avl_node, left_node));
            _delete_nodeT(target_node);
        }
        else
        {
            // Deleting a node with both children -- Hardest one to code
            // Find largest node in the left subtree
            Vec *target_to_largest_left = Vec_with_capacity(target_node->height); // Holds AvlTrees
            AvlTreeT *temp_tree = target_node->left;
            while (Option_is_some(temp_tree->avl_node))
            {
                Vec_push(target_to_largest_left, Obj_from_ptr(temp_tree));
                AvlNodeT *temp_node = Option_get(Option_clone(temp_tree->avl_node));
                temp_tree = temp_node->right;
            } // Largest node is at the top of the vec stack

            // temp_tree should be set to a null node, but we can't delete because it's
            // properly attached to another node, so we can just ignore temp_tree.
            AvlTreeT *largest_tree_before_target = Obj_to_ptr(Option_obj_get(Vec_pop(target_to_largest_left)));
            AvlNodeT *largest_node_before_target = Option_get(Option_clone(largest_tree_before_target->avl_node));

            // Can't have a right node, since that was the condition for stopping the earlier loop.
            // Can't have both nodes, since we can't have the right node.
            // Therefore, we can only have a left node, or no nodes.
            if (!(Option_is_some(largest_node_before_target->left->avl_node) || Option_is_some(largest_node_before_target->right->avl_node)))
            {
                // Replace the data only, to prevent having to reconnect the node leaves.
                Option_insert(return_data, _replace_ptr(&target_node->data, largest_node_before_target->data));
                Option_delete(Option_take(largest_tree_before_target->avl_node));
                _delete_nodeT(largest_node_before_target);
            }
            else
            {                
                // Still replace the data
                Option_insert(return_data, _replace_ptr(&target_node->data, largest_node_before_target->data));

                // Now replace the node we just use to replace the data, and replace that with its left node
                AvlNodeT *left_node = Option_get(Option_clone(largest_node_before_target->left->avl_node));
                Option_delete(Option_replace(largest_tree_before_target->avl_node, left_node));
                _delete_nodeT(largest_node_before_target);
            }

            // Update node heights, rebalance
            Iter_for_each_obj(Vec_iter_reverse(target_to_largest_left, true), ^ void (ObjWrap tree_ptr)
            {
                AvlTreeT *tree = Obj_to_ptr(tree_ptr);
                AvlNodeT *node = Option_get(Option_clone(tree->avl_node));
                AvlNodeT_update_height(node);
                AvlNodeT_rebalance(node);
            });
        }
        Iter_for_each_obj(Vec_iter_reverse(prev_node_trees, false), ^ void (ObjWrap tree_ptr) 
        {
            AvlTreeT *tree = Obj_to_ptr(tree_ptr);
            AvlNodeT *node = Option_get(Option_clone(tree->avl_node));
            AvlNodeT_update_height(node);
            AvlNodeT_rebalance(node);
        });

        return return_data;
    }
    else return target_tree_opt;
}

Option *AvlTreeSet_remove(const AvlTreeSet *self, const void *item)
{
    Vec *prev_node_trees = Vec_new(); // Stores AvlTrees, not AvlNodes
    AvlTreeT *curr_tree = self->root;
    Option *target_tree = Option_of(NULL);

    while (!Option_is_some(target_tree) && Option_is_some(curr_tree->avl_node))
    {
        AvlNodeT *curr_node = Option_get(Option_clone(curr_tree->avl_node));
        const Ordering ord = self->table->cmp(curr_node->data, item);
        switch (ord)
        {
            case Less:
                Vec_push(prev_node_trees, Obj_from_ptr(curr_tree));
                curr_tree = curr_node->right;
                break;
            case Greater:
                Vec_push(prev_node_trees, Obj_from_ptr(curr_tree));
                curr_tree = curr_node->left;
                break;
            case Equal:
                Option_insert(target_tree, curr_tree);
                break;
        }
    }

    Option *removed_item = _remove_nodeT_from_treeT(prev_node_trees, target_tree);
    Vec_delete(&prev_node_trees);

    return removed_item;
}

// bool AvlTreeSet_equal(AvlTreeSet *self, AvlTreeSet *other)
// {
//     __block bool was_different = false;
//     Iter_for_each_zip(AvlTreeSet_iter(self, false), AvlTreeSet_iter(other, false),
//             ^ void (ObjWrap item1, ObjWrap item2)
//     {
//         size_t item1_hack = Obj_to_size(item1);
//         size_t item2_hack = Obj_to_size(item2);
//         Ordering ord = self->table->cmp(&item1_hack, &item2_hack);
//         if (ord != Equal)
//         {
//             was_different = true;
//         }
//     });
//
//     return !was_different;
// }

//
// bool AvlTreeSetConstr_push(void *tree_set, void *item)
// {
//     return AvlTreeSet_insert(tree_set, item);
// }
//
// static const ConstructorVTable constr_vtable = {
//     .push = AvlTreeSetConstr_push,
// };
//
// Constructor AvlTreeSet_constr(const OrderingVTable *ordering)
// {
//     AvlTreeSet *tree = AvlTreeSet_new(ordering);
//     Constructor constr = {
//         .vtable = &constr_vtable,
//         .collection = tree,
//     };
//     return constr;
// }

// ---- AvlNode ---- //

bool AvlNodeT_rebalance(AvlNodeT *self)
{
    switch (AvlNodeT_balance_factor(self))
    {
        case -2:
        {
            AvlNodeT *right = Option_get(Option_clone(self->right->avl_node));
            if (AvlNodeT_balance_factor(right) == 1)
            {
                AvlNodeT_rotate_right(right);
            }
            AvlNodeT_rotate_left(self);
            return true;
        }
        case 2:
        {
            AvlNodeT *left = Option_get(Option_clone(self->left->avl_node));
            if (AvlNodeT_balance_factor(left) == -1)
            {
                AvlNodeT_rotate_left(left);
            }
            AvlNodeT_rotate_right(self);
            return true;
        }
        default:
        {
            return false;
        }
    }
}

size_t AvlNodeT_left_height(const AvlNodeT *self) 
{
    return Option_map_or_size(Option_clone(self->left->avl_node), 0, ^ size_t (void *left_node) { 
        return ((AvlNodeT *) left_node)->height;
    });
}

size_t AvlNodeT_right_height(const AvlNodeT *self)
{
    return Option_map_or_size(Option_clone(self->right->avl_node), 0, ^ size_t (void *right_node) {
        return ((AvlNodeT *) right_node)->height;
    });
}

int8_t AvlNodeT_balance_factor(const AvlNodeT *self)
{
    size_t left_height = AvlNodeT_left_height(self);
    size_t right_height = AvlNodeT_right_height(self);

    if (left_height >= right_height)
    {
        return (left_height - right_height);
    }
    else 
    {
        return -(right_height - left_height);
    }
}

void AvlNodeT_update_height(AvlNodeT *self)
{
    self->height = 1 + max_size(AvlNodeT_left_height(self), AvlNodeT_right_height(self));
}

bool AvlNodeT_rotate_right(AvlNodeT *self)
{
    if (!Option_is_some(self->left->avl_node))
    {
        return false;
    }

    // Grab nodes
    AvlNodeT *d_node = self; // To not drive me crazy later
    AvlNodeT *b_node = Option_get(Option_clone(d_node->left->avl_node));
    Option *e_node_opt = d_node->right->avl_node; // Might not exist (probably doesn't, now that I think about it)
    AvlNodeT *a_node = Option_get(Option_clone(b_node->left->avl_node));
    Option *c_node_opt = b_node->right->avl_node; // Might not exist

    // Swap data from B and D
    _swap_ptrs(&b_node->data, &d_node->data);

    // Perform re-linking (rotation)
    Option_delete(Option_replace(d_node->left->avl_node, a_node)); // Replace creates an option object, 
                                                                   // so we need to delete it to not leak
    if (Option_is_some(e_node_opt))
    {
        AvlNodeT *e_node = Option_get(Option_clone(e_node_opt));
        Option_delete(Option_replace(b_node->right->avl_node, e_node));
    }
    else
    {
        Option_delete(Option_take(b_node->right->avl_node));
    } 
    if (Option_is_some(c_node_opt))
    {
        AvlNodeT *c_node = Option_get(Option_clone(c_node_opt));
        Option_delete(Option_replace(b_node->left->avl_node, c_node));
    }
    else
    {
        Option_delete(Option_take(b_node->left->avl_node));
    }
    Option_delete(Option_replace(d_node->right->avl_node, b_node));

    AvlNodeT_update_height(b_node);
    AvlNodeT_update_height(d_node);

    return true;
}

bool AvlNodeT_rotate_left(AvlNodeT *self)
{
    if (!Option_is_some(self->right->avl_node))
    {
        return false;
    }

    // Grab nodes
    AvlNodeT *d_node = self; // To not drive me crazy later
    AvlNodeT *b_node = Option_get(Option_clone(d_node->right->avl_node));
    Option *e_node_opt = d_node->left->avl_node; // Might not exist (probably doesn't, now that I think about it)
    AvlNodeT *a_node = Option_get(Option_clone(b_node->right->avl_node));
    Option *c_node_opt = b_node->left->avl_node; // Might not exist

    // Swap data from B and D
    _swap_ptrs(&b_node->data, &d_node->data);

    // Perform re-linking (rotation)
    Option_delete(Option_replace(d_node->right->avl_node, a_node)); // Replace creates an option object, 
                                                                    // so we need to delete it to not leak
    if (Option_is_some(e_node_opt))
    {
        AvlNodeT *e_node = Option_get(Option_clone(e_node_opt));
        Option_delete(Option_replace(b_node->left->avl_node, e_node));
    }
    else
    {
        Option_delete(Option_take(b_node->left->avl_node));
    }
    if (Option_is_some(c_node_opt))
    {
        AvlNodeT *c_node = Option_get(Option_clone(c_node_opt));
        Option_delete(Option_replace(b_node->right->avl_node, c_node));
    }
    else
    {
        Option_delete(Option_take(b_node->right->avl_node));
    }
    Option_delete(Option_replace(d_node->left->avl_node, b_node));

    AvlNodeT_update_height(b_node);
    AvlNodeT_update_height(d_node);

    return true;
}


// ---- Iterator ---- //

Option_obj *AvlTreeSetIter_next(void *self_as_void_ptr)
{
    AvlTreeSetIter *self = self_as_void_ptr;
    while (Option_is_some(self->curr_tree->avl_node))
    {
        Option *as_ref = Option_clone(self->curr_tree->avl_node);
        AvlNodeT *curr_node = Option_get(as_ref);
        Vec_push(self->prev_nodes, Obj_from_ptr(curr_node));
        self->curr_tree = curr_node->left;
    }

    if (!Vec_empty(self->prev_nodes))
    {
        Option_obj *pop = Vec_pop(self->prev_nodes);
        AvlNodeT *popped_node = Obj_to_ptr(Option_obj_get(pop));
        self->curr_tree = popped_node->right;
        // printf("Right before leaving AvlTreeSetIter_next: data = %p\n", popped_node->data);
        return Option_obj_of(Obj_from_ptr(popped_node->data));
    }
    else
    {
        return Option_obj_empty();
    }
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
    Vec_delete(&self->prev_nodes);
    if (delete_collection)
    {
        AvlTreeSet_delete(&self->tree_set);
    }
    free(self);
}

static const IteratorVTable AvlTreeSet_iter_vtable = {
    .next = ^ Option_obj *(void *self) {
        return AvlTreeSetIter_next(self);
    },
    .delete_iterable_obj = ^ void (void *self, bool delete_collection) {
        AvlTreeSetIter_delete(self, delete_collection);
    }
};

Iterator AvlTreeSet_iter(AvlTreeSet *self, bool delete_collection_after_iter)
{
    return (Iterator) {
        .vtable = AvlTreeSet_iter_vtable,
        .delete_collection_after_iter = delete_collection_after_iter,
        .iterable_obj = AvlTreeSetIter_from(self)
    };
}



// ---- Set wrapper functions ---- //

bool _avl_set_contains(const void *set, const void *item) { return AvlTreeSet_contains(set, item); }
Iterator _avl_set_iter(void *set, bool delete_collection_after_iter) 
{ 
    return AvlTreeSet_iter(set, delete_collection_after_iter); 
}
bool _avl_set_push(void *set, void *item) { return AvlTreeSet_insert(set, item); }
bool _avl_set_empty(const void *set) { return AvlTreeSet_is_empty(set); }
// bool _avl_set_equal(void *set, void *other) { return AvlTreeSet_equal(set, other); }
void _avl_set_delete(void *set) { AvlTreeSet_delete((AvlTreeSet **) &set); }
Option *_avl_set_remove(void *set, const void *item) { return AvlTreeSet_remove(set, item); }

static const SetVTable AvlTreeSet_set_vtable = {
    .contains = _avl_set_contains,
    .iter = _avl_set_iter,
    .push = _avl_set_push,
    .empty = _avl_set_empty,
    .difference_of = NULL,
    // .equal = _avl_set_equal,
    .delete = _avl_set_delete,
    .intersection_of = NULL,
    .pop = NULL,
    .remove = _avl_set_remove,
    .union_of = NULL,
};

Set AvlTreeSet_as_set(AvlTreeSet *self) 
{
    Set set = {
        .set = self,
        .vtable = &AvlTreeSet_set_vtable,
    };
    return set;
}
