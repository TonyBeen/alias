/*************************************************************************
    > File Name: rbtree.cpp
    > Author: hsz
    > Brief:
    > Created Time: Sun 12 Dec 2021 11:11:06 PM CST
 ************************************************************************/

#include "rbtree.h"
#include "Errors.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static void rbtree_insert_fixup(rbtree_t *tree, rbtree_node_t *node);
static void rbtree_left_rotate(rbtree_node_t **root, rbtree_node_t *node);
static void rbtree_right_rotate(rbtree_node_t **root, rbtree_node_t *node);

static bool rbtree_insert_value(rbtree_t *tree, rbtree_node_t *node)
{
    assert(tree && node);
    rbtree_node_t *curr = tree->root;
    rbtree_node_t *parent = NULL;

    if (curr == tree->null) { // 无节点时
        tree->root = node;
        tree->root->left = tree->null;
        tree->root->right = tree->null;
        ++tree->size;
        return true;
    }

    while (curr != tree->null) {
        if (node->key == curr->key) {
            return false;
        }
        parent = curr;
        if (node->key < curr->key) {
            curr = curr->left;
        }
        if (node->key > curr->key) {
            curr = curr->right;
        }
    }

    if(node->key < parent->key) { // 左子树
        parent->left = node;
    } else { // 右子树
        parent->right = node;
    }
    if (rbt_is_red(parent)) {
        rbt_black(node);
    } else {
        rbt_red(node);
    }
    return true;
}

int rbtree_insert(rbtree_t *tree, const RBTREE_TYPE &key)
{
    if (!tree) {
        return Jarvis::INVALID_PARAM;
    }

    rbtree_node_t *temp = (rbtree_node_t *)malloc(sizeof(rbtree_node_t));
    if (!temp) {
        return Jarvis::NO_MEMORY;
    }
    memset(temp, 0, sizeof(rbtree_node_t));
    rbt_red(temp);
    temp->key = key;
    temp->left = tree->null;
    temp->right = tree->null;
    if (rbtree_insert_value(tree, temp) == false) {
        free(temp);
        return Jarvis::ALREADY_EXISTS;
    }
    rbtree_insert_fixup(tree, temp);
}

int rbtree_erase(rbtree_t *tree, rbtree_node_t *node)
{

}

rbtree_node_t *rbtree_find(rbtree_t *tree, RBTREE_TYPE key)
{

}

// node为插入节点
static void rbtree_insert_fixup(rbtree_t *tree, rbtree_node_t *node)
{
    rbtree_node_t *root = tree->root;
    rbtree_node_t *uncle = NULL;
    while (node != root && rbt_is_red(node)) {
        if (node->parent == node->parent->parent->left) {   // 父节点是祖父节点的左节点
            uncle = node->parent->parent->right;
            if (rbt_is_red(uncle)) {
                rbt_black(node->parent);
                rbt_black(uncle);
                rbt_red(uncle->parent);
                node = uncle->parent;
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    rbtree_left_rotate(&root, node);
                }
                rbt_black(node->parent);
                rbt_red(node->parent->parent);
                rbtree_right_rotate(&root, node->parent->parent);
            }
        } else {    // 父节点是祖父节点的右节点
            uncle = node->parent->parent->left;
            if (rbt_is_red(uncle)) {
                rbt_black(node->parent);
                rbt_black(uncle);
                rbt_red(node->parent->parent);
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    rbtree_right_rotate(&root, node);
                }
                rbt_black(node->parent);
                rbt_red(node->parent->parent);
                rbtree_left_rotate(&root, node->parent->parent);
            }
        }
    }
}

// 以node节点左旋
static void rbtree_left_rotate(rbtree_node_t **root, rbtree_node_t *node)
{
    rbtree_node_t *temp = NULL;
    temp = node->right;

    node->right = temp->left;
    if (temp->left != &gNilNode) {  // nil节点不能设置父节点指向
        temp->left->parent = node;
    }
    temp->parent = node->parent;

    if (node == *root) {
        *root = temp;
    } else if (node == node->parent->left) {    // 更新node父节点的左子树指向
        node->parent->left = temp;
    } else if (node == node->parent->right) {   // 更新node父节点的右子树指向
        node->parent->right = temp;
    }

    temp->left = node;
    node->parent = temp;
}

// 以node节点右旋
static void rbtree_right_rotate(rbtree_node_t **root, rbtree_node_t *node)
{
    rbtree_node_t *temp = NULL;
    temp = node->left;
    node->left = temp->right;
    if (temp->right != &gNilNode) {
        temp->right->parent = node;
    }

    if (node == *root) {
        *root = temp;
    } else if (node == node->parent->left) {
        node->parent->left = temp;
    } else if (node == node->parent->right) {
        node->parent->right = temp;
    }

    temp->right = node;
    node->parent = temp;
}

#ifdef __cplusplus
}
#endif