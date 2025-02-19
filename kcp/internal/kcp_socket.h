/*************************************************************************
    > File Name: kcp_socket.h
    > Author: hsz
    > Brief:
    > Created Time: 2025年02月19日 星期三 17时05分24秒
 ************************************************************************/

#ifndef __KCP_SOCKET_H__
#define __KCP_SOCKET_H__

#include "kcp_def.h"
#include "socket_set.h"
#include "kcp_config.h"
#include "kcpp.h"

EXTERN_C_BEGIN

typedef struct KcpFunctionCallback {
    on_kcp_accept_t     on_accepted;
    on_kcp_connected_t  on_connected;
    on_kcp_closed_t     on_closed;
} kcp_function_callback_t;

typedef struct KcpConnection {
    socket_set_node_t   socket_node;
    struct KcpContext*  kcp_ctx;
} kcp_connection_t;

struct KcpContext {
    socket_t                    sock;
    kcp_function_callback_t     callback;
    kcp_config_t                config;

    socket_set_t                socket_set;
    struct event_base*          event_base;
    void*                       user_data;
};

EXTERN_C_END

#endif // __KCP_SOCKET_H__
