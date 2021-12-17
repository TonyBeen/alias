/*************************************************************************
    > File Name: rbtree.h
    > Author: hsz
    > Brief:
    > Created Time: Sat 11 Dec 2021 01:26:52 PM CST
 ************************************************************************/

#ifndef __RBTREE_H__
#define __RBTREE_H__

#include "sysdef.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
    COLOR_BLACK = 0,
    COLOR_RED = 1
};

#define rbt_red(node)       (node->color =  COLOR_RED)
#define rbt_black(node)     (node->color =  COLOR_BLACK)
#define rbt_is_red(node)    (node->color == COLOR_RED)
#define rbt_is_black(node)  (node->color == COLOR_BLACK)

#ifndef RBTREE_TYPE
#define RBTREE_TYPE int
#endif

struct rbtree_node_t {
    uint8_t color;
    RBTREE_TYPE key;

    // rbtree branch
    rbtree_node_t *parent;
    rbtree_node_t *left;
    rbtree_node_t *right;
};

static rbtree_node_t gNilNode = {COLOR_BLACK, 0, NULL, NULL, NULL};

struct rbtree_s {
    rbtree_node_t *root;   // rbtree root
    rbtree_node_t *null;   // rbtree nil node
    uint32_t    size;       // rbtree node size
};
typedef struct rbtree_s rbtree_t;

static inline void rbtree_init(rbtree_t *tree)
{
    tree->root = &gNilNode;
    tree->null = &gNilNode;
    tree->size = 0;
    rbt_black(tree->root);
}

int rbtree_insert(rbtree_t *tree, const RBTREE_TYPE &key);
int rbtree_erase(rbtree_t *tree, rbtree_node_t *node);
rbtree_node_t *rbtree_find(rbtree_t *tree, RBTREE_TYPE key);

#ifdef __cplusplus
}
#endif

#endif // __RBTREE_H__