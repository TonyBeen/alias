#ifndef __KCP_INTERNAL_SOCKET_SET_H__
#define __KCP_INTERNAL_SOCKET_SET_H__

#include <stdbool.h>

#include "kcp_def.h"
#include "rbtree.h"
#include "kcp_protocol.h"

typedef struct SocketSetNode {
    struct rb_node      node;
    struct KcpSocket*   sock;
} socket_set_node_t;

typedef struct rb_root conversation_set_t;

EXTERN_C_BEGIN

socket_set_node_t *socket_set_search(conversation_set_t *root, int32_t conv);

bool socket_set_insert(conversation_set_t *root, socket_set_node_t *node);

socket_set_node_t *socket_set_erase(conversation_set_t *root, int32_t conv);

void socket_set_erase_node(conversation_set_t *root, socket_set_node_t *node);

EXTERN_C_END

#endif // __KCP_INTERNAL_SOCKET_SET_H__
