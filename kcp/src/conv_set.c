#include "conv_set.h"
#include <assert.h>

socket_set_node_t *socket_set_search(conversation_set_t *root, int32_t conv)
{
    struct rb_node *node = root->rb_node;
  	while (node) {
  		socket_set_node_t *pthis = container_of(node, socket_set_node_t, node);
        assert(pthis->sock != NULL);

		if (conv < pthis->sock->conv) {
            node = node->rb_left;
        } else if (conv > pthis->sock->conv) {
  			node = node->rb_right;
        } else {
  			return pthis;
        }
	}

	return NULL;
}

bool socket_set_insert(conversation_set_t *root, socket_set_node_t *data)
{
    if (root == NULL || data == NULL || data->sock == NULL) {
        return false;
    }

    struct rb_node **new = &(root->rb_node), *parent = NULL;
    while (*new) {
        socket_set_node_t *pthis = container_of(*new, socket_set_node_t, node);
        assert(pthis->sock != NULL);

        parent = *new;
        if (data->sock->conv < pthis->sock->conv) {
            new = &((*new)->rb_left);
        } else if (data->sock->conv > pthis->sock->conv) {
            new = &((*new)->rb_right);
        } else {
            return false;
        }
    }

    rb_link_node(&data->node, parent, new);
    rb_insert_color(&data->node, root);
    return true;
}

socket_set_node_t *socket_set_erase(conversation_set_t *root, int32_t conv)
{
    if (root == NULL) {
        return NULL;
    }

    socket_set_node_t *pthis = socket_set_search(root, conv);
    if (pthis) {
        rb_erase(&pthis->node, root);
    }

    return pthis;
}

void socket_set_erase_node(conversation_set_t *root, socket_set_node_t *node)
{
    if (root != NULL && node != NULL) {
        rb_erase(&node->node, root);
    }
}