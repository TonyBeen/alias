#include "kcpp.h"

#include <event2/event.h>

#include "kcp_error.h"
#include "kcp_protocol.h"

struct KcpContext *kcp_create(struct event_base *base, void *user)
{
    if (base == NULL) {
        return NULL;
    }

    struct KcpContext *ctx = (struct KcpContext *)malloc(sizeof(struct KcpContext));
    if (ctx == NULL) {
        return NULL;
    }

    ctx->sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (ctx->sock == INVALID_SOCKET) {
        free(ctx);
        return NULL;
    }
    ctx->callback = (kcp_function_callback_t) {
        .on_accepted = NULL,
        .on_connected = NULL,
        .on_closed = NULL
    };

    ctx->socket_set.rb_node = NULL;
    ctx->event_base = base;
    ctx->user_data = user;
    return ctx;
}

void kcp_destroy(struct KcpContext *kcp_ctx)
{
    if (kcp_ctx == NULL) {
        return;
    }

    // TODO 释放资源

#if defined(OS_LINUX)
    close(kcp_ctx->sock);
#else
    closesocket(kcp_ctx->sock);
#endif

    free(kcp_ctx);
}

int32_t kcp_configure(struct KcpConnection *kcp_connection, config_key_t flags, kcp_config_t *config)
{
    if (kcp_connection == NULL || config == NULL) {
        return INVALID_PARAM;
    }

    if (flags & CONFIG_KEY_NODELAY) {
        kcp_connection->socket_node.sock->nodelay = config->nodelay ? 1 : 0;
        if (kcp_connection->socket_node.sock->nodelay) {
            kcp_connection->socket_node.sock->rx_minrto = IKCP_RTO_NDL;
        } else {
            kcp_connection->socket_node.sock->rx_minrto = IKCP_RTO_MIN;
        }
    }

    if (flags & CONFIG_KEY_INTERVAL) {
        if (config->interval < KCP_INTERVAL_MIN || config->interval > KCP_INTERVAL_MAX) {
            return INVALID_PARAM;
        }

        kcp_connection->socket_node.sock->interval = config->interval;
    }

    if (flags & CONFIG_KEY_RESEND) {
        if (config->resend > KCP_FASTACK_LIMIT) {
            return INVALID_PARAM;
        }

        kcp_connection->socket_node.sock->fastresend = config->resend;
    }

    if (flags & CONFIG_KEY_NC) {
        kcp_connection->socket_node.sock->nocwnd = config->nc ? 1 : 0;
    }

    return NO_ERROR;
}

int32_t kcp_bind(struct KcpContext *kcp_ctx, const sockaddr_t *addr, const char *nic)
{
    if (kcp_ctx == NULL || addr == NULL) {
        return INVALID_PARAM;
    }

    if (bind(kcp_ctx->sock, (const struct sockaddr *)addr, sizeof(sockaddr_t)) == SOCKET_ERROR) {
        return BIND_ERROR;
    }

    return NO_ERROR;
}
