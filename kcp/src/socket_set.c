#include "socket_set.h"
#include <assert.h>

socket_set_node_t *socket_set_search(socket_set_t *root, int32_t conv)
{
    struct rb_node *node = root->rb_node;
  	while (node) {
  		socket_set_node_t *this = container_of(node, socket_set_node_t, node);
        assert(this->sock != NULL);

		if (conv < this->sock->conv) {
            node = node->rb_left;
        } else if (conv > this->sock->conv) {
  			node = node->rb_right;
        } else {
  			return this;
        }
	}

	return NULL;
}

bool socket_set_insert(socket_set_t *root, socket_set_node_t *data)
{
    if (root == NULL || data == NULL || data->sock == NULL) {
        return false;
    }

    struct rb_node **new = &(root->rb_node), *parent = NULL;
    while (*new) {
        socket_set_node_t *this = container_of(*new, socket_set_node_t, node);
        assert(this->sock != NULL);

        parent = *new;
        if (data->sock->conv < this->sock->conv) {
            new = &((*new)->rb_left);
        } else if (data->sock->conv > this->sock->conv) {
            new = &((*new)->rb_right);
        } else {
            return false;
        }
    }

    rb_link_node(&data->node, parent, new);
    rb_insert_color(&data->node, root);
    return true;
}

socket_set_node_t *socket_set_erase(socket_set_t *root, int32_t conv)
{
    if (root == NULL) {
        return NULL;
    }

    socket_set_node_t *this = socket_set_search(root, conv);
    if (this) {
        rb_erase(&this->node, root);
    }

    return this;
}

void socket_set_erase_node(socket_set_t *root, socket_set_node_t *node)
{
    if (root != NULL && node != NULL) {
        rb_erase(&node->node, root);
    }
}