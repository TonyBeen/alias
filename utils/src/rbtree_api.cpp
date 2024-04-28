/*************************************************************************
    > File Name: rbtree_api.cpp
    > Author: hsz
    > Brief:
    > Created Time: Wed 23 Nov 2022 03:56:21 PM CST
 ************************************************************************/

#include "utils/rbtree_api.h"

#include <assert.h>

void rbtree_init(rb_root *root)
{
    if (root) {
        root->rb_node = nullptr;
    }
}

void rbtree_clear(rb_root *root, void (*free_node_cb)(rb_node *))
{
    if (root == nullptr) {
        return;
    }
    struct rb_node *node = nullptr;
    while ((node = root->rb_node) != nullptr) {
        rb_erase(node, root);
        if (eular_likely(free_node_cb != nullptr)) {
            free_node_cb(node);
        }
    }
    root->rb_node = nullptr;
}

rb_node *rbtree_insert(rb_root *root, rb_node *newNode, int (*compare_callback)(rb_node *, rb_node *))
{
    if (root == nullptr || compare_callback == nullptr) {
        return nullptr;
    }

    struct rb_node **node = &(root->rb_node);
    struct rb_node *parent = nullptr;
    bool exists = false;
    int compareResult = 0;
    while (nullptr != (*node)) {
        parent = *node;
        compareResult = compare_callback(parent, newNode);
        if (compareResult < 0) { // parent < newNode
            node = &(*node)->rb_right;
        } else if (compareResult > 0) {
            node = &(*node)->rb_left;
        } else {
            exists = true;
            break;
        }
    }

    if (!exists) {
        rb_link_node(newNode, parent, node);
        rb_insert_color(newNode, root);
    }

    return exists ? nullptr : newNode;
}

rb_node *rbtree_erase(rb_root *root, const void *seed, int (*equal_callback)(rb_node *, const void *))
{
    if (root == nullptr || seed == nullptr || equal_callback == nullptr) {
        return nullptr;
    }

    struct rb_node *eraseNode = rbtree_search(root, seed, equal_callback);
    if (eraseNode != nullptr) {
        rb_erase(eraseNode, root);
    }

    return eraseNode;
}

bool isValidNode(rb_root *root, rb_node *node)
{
    assert(root && node);
    rb_node *parent = node;
    bool valid = false;
    while (parent) {
        if (parent == root->rb_node) {
            valid = true;
            break;
        }
        if (parent == parent->rb_parent) { // 需确定根节点的rb_parent是否为空，不为空则会死循环
            break;
        }
        parent = parent->rb_parent;
    }

    return valid;
}

rb_node *rbtree_erase_node(rb_root *root, rb_node *node)
{
    if (root == nullptr || node == nullptr) {
        return nullptr;
    }
    assert(isValidNode(root, node));
    
    rb_erase(node, root);
    return node;
}

bool rbtree_foreach(rb_root *root, bool (*data_callback)(rb_node *node))
{
    if (root == nullptr || data_callback == nullptr) {
        return false;
    }

    struct rb_node *begin = rb_first(root);
    struct rb_node *next = begin;
    struct rb_node *end = rb_last(root);

    if (begin == nullptr) { // no nodes
        return true;
    }

    // NOTE 由于遍历时是有序的，故保存next后，即使在回调中删除begin节点也不会导致循环出现问题，因为下一个节点本就应该是next
    while (begin != end) {
        next = rb_next(next);
        if (data_callback(begin)) {
            return true;
        }
        begin = next;
    }

    data_callback(end);
    return true;
}

rb_node *rbtree_search(rb_root *root, const void *seed, int (*equal_callback)(rb_node *, const void *))
{
    if (root == nullptr || equal_callback == nullptr) {
        return nullptr;
    }

    struct rb_node *curr = root->rb_node;
    bool exist = false;
    int compareResult = 0;
    while (curr) {
        compareResult = equal_callback(curr, seed);
        if (compareResult < 0) {
            curr = curr->rb_left;
        } else if (compareResult > 0) {
            curr = curr->rb_right;
        } else {
            exist = true;
            break;
        }
    }

    return exist ? curr : nullptr;
}

size_t rbtree_size(rb_root *root)
{
    if (root == nullptr) {
        return 0;
    }

    size_t count = 0;
    struct rb_node *begin = rb_first(root);
    struct rb_node *end = rb_last(root);

    if (begin == nullptr) {
        return 0;
    }

    while (begin != end) {
        begin = rb_next(begin);
        ++count;
    }

    ++count; // for end
    return count;
}