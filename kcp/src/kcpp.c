#include "kcpp.h"

#include <event2/event.h>

#include "kcp_error.h"
#include "socket_set.h"

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

    ctx->config = KCP_CONFIG_NORMAL;
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

int32_t kcp_configure(struct KcpContext *kcp_ctx, config_key_t flags, kcp_config_t *config)
{
    if (kcp_ctx == NULL || config == NULL) {
        return INVALID_PARAM;
    }

    if (flags & CONFIG_KEY_NODELAY) {
        kcp_ctx->config.nodelay = config->nodelay ? 1 : 0;
    }

    if (flags & CONFIG_KEY_INTERVAL) {
        if (config->interval < KCP_INTERVAL_MIN || config->interval > KCP_INTERVAL_MAX) {
            return INVALID_PARAM;
        }

        kcp_ctx->config.interval = config->interval;
    }

    if (flags & CONFIG_KEY_RESEND) {
        if (config->resend < KCP_RESEND_MIN || config->resend > KCP_RESEND_MAX) {
            return INVALID_PARAM;
        }

        kcp_ctx->config.resend = config->resend;
    }

    if (flags & CONFIG_KEY_NC) {
        kcp_ctx->config.nc = config->nc ? 1 : 0;
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
