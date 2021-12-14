/*************************************************************************
    > File Name: rbtree.cpp
    > Author: hsz
    > Brief:
    > Created Time: Sun 12 Dec 2021 11:11:06 PM CST
 ************************************************************************/

#include "rbtree.h"
#include "Errors.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

static void rbtree_insert_value(rbtree_t *tree, rbtree_node_t *node)
{
    if (!tree || !node) {
        return;
    }
    
    rbtree_node_t *curr = tree->root;
    rbtree_node_t *parent = NULL;

    if (curr == tree->null) { // 只有根节点
        tree->root = node;
        tree->root->left = tree->null;
        tree->root->right = tree->null;
        ++tree->size;
        return;
    }

    while (curr != tree->null) {
        if (node->key == curr->key) {
            return;
        }
        parent = curr;
        if (node->key < curr->key) {
            curr = curr->left;
        }
        if (node->key > curr->key) {
            curr = curr->right;
        }
    }

    // TODO 该插入节点了
    
}

int rbtree_insert(rbtree_t *tree, RBTREE_TYPE key)
{
    if (!tree) {
        return Jarvis::INVALID_PARAM;
    }

    rbtree_node_t *temp = (rbtree_node_t *)malloc(sizeof(rbtree_node_t));
    if (!temp) {
        return Jarvis::NO_MEMORY;
    }
    rbt_red(temp);
    temp->key = key;

    
}

int rbtree_erase(rbtree_t *tree, rbtree_node_t *node)
{

}

rbtree_node_t *rbtree_find(rbtree_t *tree, RBTREE_TYPE key)
{

}

#ifdef __cplusplus
}
#endif