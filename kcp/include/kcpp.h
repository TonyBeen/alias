#ifndef __KCP_PLUS_H__
#define __KCP_PLUS_H__

#include <stdint.h>
#include <stddef.h>

#include <kcp_def.h>
#include <kcp_config.h>
#include <kcp_net_def.h>

EXTERN_C_BEGIN

struct KcpContext;
struct KcpConnection;
struct event;
struct event_base;
struct iovec;

/**
 * @brief Create a new kcp control block.
 *
 * @param base event base.
 * @param user The user data.
 * @return struct KcpContext*
 */
struct KcpContext *kcp_create(struct event_base *base, void *user);

/**
 * @brief destroy kcp control block.
 *
 * @param kcp The kcp control block.
 */
void kcp_destroy(struct KcpContext *kcp);

/**
 * @brief configure kcp control block.
 *
 * @param kcp The kcp control block.
 * @param config The kcp configuration.
 *
 * @return int32_t 0 if success, otherwise -1.
 */
int32_t kcp_configure(struct KcpContext *kcp, config_key_t flags, kcp_config_t *config);

/**
 * @brief 绑定本地地址和端口, 网卡
 *
 * @param kcp kcp控制块
 * @param host 本地地址
 * @param port 本地端口
 * @param nic 网卡名字
 * @return int32_t 成功返回0, 否则返回负值
 */
int32_t kcp_bind(struct KcpContext *kcp, const sockaddr_t *addr, const char *nic);

/**
 * @brief 
 *
 * @param kcp 
 * @param addr 
 * @param addrlen 
 * @return struct KcpConnection* 
 */
struct KcpConnection *kcp_accept(struct KcpContext *kcp, sockaddr_t *addr);

/**
 * @brief 
 * 
 * @param kcp 
 * @param host 
 * @param port 
 * @param timeout_ms 
 * @return struct KcpConnection* 
 */
struct KcpConnection *kcp_connect(struct KcpContext *kcp, const char *host, uint16_t port, uint32_t timeout_ms);

/**
 * @brief Send FIN to peer.
 */
void kcp_close(struct KcpConnection *kcp);

/**
 * @brief Send RST to peer.
 */
void kcp_shutdown(struct KcpConnection *kcp);

/**
 * @brief send data to peer.
 * @param kcp The kcp control block.
 * @param data The data to send.
 * @param size The size of data.
 * 
 * @return int32_t Return the byte size written to the sending queue.
 */
int32_t kcp_write(struct KcpContext *kcp, const void *data, size_t size);

int32_t kcp_writev(struct KcpContext *kcp, const struct iovec *iov, int32_t iovcnt);

int32_t kcp_recd(struct KcpContext *kcp, void *data, size_t size);

int32_t kcp_recdv(struct KcpContext *kcp, struct iovec *iov, int32_t iovcnt);

int32_t kcp_update(struct KcpContext *kcp, uint32_t current);

EXTERN_C_END

#endif // __KCP_PLUS_H__
