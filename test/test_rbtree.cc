/*************************************************************************
    > File Name: test_rbtree.cc
    > Author: hsz
    > Brief:
    > Created Time: Fri 17 Dec 2021 10:56:23 AM CST
 ************************************************************************/

#include <string>
#include <utils/rbtree.h>
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
    rbtree_t rbtree;
    rbtree_init(&rbtree);
    rbtree_insert(&rbtree, 1);
    rbtree_insert(&rbtree, 2);
    rbtree_insert(&rbtree, 3);
    rbtree_insert(&rbtree, 4);
    rbtree_insert(&rbtree, 5);

    return 0;
}
