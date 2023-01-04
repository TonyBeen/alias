/*************************************************************************
    > File Name: test_map_node.cc
    > Author: hsz
    > Brief:
    > Created Time: Mon 28 Nov 2022 04:29:58 PM CST
 ************************************************************************/

#include <utils/map_node.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>

using namespace std;

int main(int argc, char **argv)
{
    map_private::MapData<int, int> *data = map_private::MapData<int, int>::create();
    printf("map size = %zu\n", data->size());

    printf("\ninsert to map(%p) 0, 1, 2\n", data);
    data->insert(0, 0);
    data->insert(1, 1);
    data->insert(2, 2);

    printf("map size = %zu\n", data->size());

    printf("foreach map\n");
    for (auto it = data->begin(); it != data->end(); it = data->nextNode(it)) {
        printf("\tkey = %d, value = %d\n", it->key, it->value);
    }

    printf("\ninsert to map(%p) 3, 4 and erase 2\n", data);
    data->insert(3, 3);
    data->insert(4, 4);
    data->erase(2);

    printf("reforeach map\n");
    for (auto it = data->rbegin(); it != data->rend(); it = data->prevNode(it)) {
        printf("\tkey = %d, value = %d\n", it->key, it->value);
    }

    printf("insert same key 3\n");
    map_private::MapNode<int, int> *node = data->insert(3, 100);
    printf("result key = %d, value = %d\n", node->key, node->value);
    assert(node->key == 3 && node->value == 3);

    data->clear();
    printf("map size = %zu\n", data->size());
    assert(data->size() == 0);
    printf("foreach map\n");
    for (auto it = data->begin(); it != data->end(); it = data->nextNode(it)) {
        printf("\tkey = %d, value = %d\n", it->key, it->value);
    }
    map_private::MapDataBase::FreeData(data);
    return 0;
}
