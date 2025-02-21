#ifndef __KCP_PLUS_H__
#define __KCP_PLUS_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <kcp_def.h>
#include <kcp_config.h>
#include <kcp_net_def.h>

EXTERN_C_BEGIN

struct KcpContext;
struct KcpConnection;
struct event;
struct event_base;
struct iovec;

/// kcp callback

/**
 * @brief kcp连接回调函数
 *
 * @param kcp_connection kcp连接, 连接失败为空
 * @param user 用户数据
 * @param code 连接结果
 */
typedef void (*on_kcp_connected_t)(struct KcpConnection *kcp_connection, void *user, int32_t code);

/**
 * @brief kcp断连回调函数
 *
 * @param kcp_connection kcp连接
 * @param user 用户数据
 * @param code 断连原因
 */
typedef void (*on_kcp_closed_t)(struct KcpConnection *kcp_connection, void *user, int32_t code);

/**
 * @brief kcp建连回调函数
 *
 * @param kcp kcp上下文
 *
 * @return bool 返回true表示接受连接, false表示拒绝连接
 */
typedef bool (*on_kcp_accept_t)(struct KcpContext *kcp_ctx);

/// kcp function

/**
 * @brief Create a new kcp control block.
 *
 * @param base event base.
 * @param user The user data.
 * @return struct KcpContext*
 */
KCP_PORT struct KcpContext *kcp_create(struct event_base *base, void *user);

/**
 * @brief destroy kcp control block.
 *
 * @param kcp The kcp control block.
 */
KCP_PORT void kcp_destroy(struct KcpContext *kcp_ctx);

/**
 * @brief configure kcp control block.
 *
 * @param kcp The kcp control block.
 * @param config The kcp configuration.
 *
 * @return int32_t 0 if success, otherwise -1.
 */
KCP_PORT int32_t kcp_configure(struct KcpConnection *kcp_connection, config_key_t flags, kcp_config_t *config);

/**
 * @brief 绑定本地地址和端口, 网卡
 *
 * @param kcp kcp控制块
 * @param host 本地地址
 * @param port 本地端口
 * @param nic 网卡名字
 * @return int32_t 成功返回0, 否则返回负值
 */
KCP_PORT int32_t kcp_bind(struct KcpContext *kcp_ctx, const sockaddr_t *addr, const char *nic);

/**
 * @brief 
 *
 * @param kcp 
 * @param addr 
 * @param addrlen 
 * @return struct KcpConnection* 
 */
KCP_PORT struct KcpConnection *kcp_accept(struct KcpContext *kcp_ctx, sockaddr_t *addr);

KCP_PORT int32_t kcp_connect(struct KcpContext *kcp_ctx, const sockaddr_t *addr, uint32_t timeout_ms, on_kcp_connected_t cb);

/**
 * @brief Send FIN to peer.
 */
KCP_PORT void kcp_close(struct KcpConnection *kcp_connection, uint32_t timeout_ms, on_kcp_closed_t cb);

/**
 * @brief Send RST to peer.
 */
KCP_PORT void kcp_shutdown(struct KcpConnection *kcp_connection);

/**
 * @brief send data to peer.
 * @param kcp The kcp control block.
 * @param data The data to send.
 * @param size The size of data.
 * 
 * @return int32_t Return the byte size written to the sending queue.
 */
KCP_PORT int32_t kcp_write(struct KcpContext *kcp_ctx, const void *data, size_t size);

KCP_PORT int32_t kcp_writev(struct KcpContext *kcp_ctx, const struct iovec *iov, int32_t iovcnt);

KCP_PORT int32_t kcp_recd(struct KcpContext *kcp_ctx, void *data, size_t size);

KCP_PORT int32_t kcp_recdv(struct KcpContext *kcp_ctx, struct iovec *iov, int32_t iovcnt);

KCP_PORT int32_t kcp_update(struct KcpContext *kcp_ctx, uint32_t current);


/**
 * @brief 获取kcp上下文的用户数据
 *
 * @param kcp_ctx kcp上下文
 * @return void* 用户数据
 */
KCP_PORT void *kcp_get_user_data(struct KcpContext *kcp_ctx);

EXTERN_C_END

#endif // __KCP_PLUS_H__
