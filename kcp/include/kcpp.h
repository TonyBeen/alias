#ifndef __KCP_PLUS_H__
#define __KCP_PLUS_H__

#include <stdint.h>
#include <stddef.h>

#include <kcp_def.h>
#include <kcp_config.h>

struct KcpControlBlock;

EXTERN_C_BEGIN

/**
 * @brief Create a new kcp control block.
 * @param conv The conversation id.
 * @param user The user data.
 *
 * @return The kcp control block.
 */
struct KcpControlBlock *kcp_create(uint32_t conv, void *user);

/**
 * @brief Send FIN to peer.
 */
void kcp_close(struct KcpControlBlock *kcp);

/**
 * @brief Send RST to peer.
 */
void kcp_shutdown(struct KcpControlBlock *kcp);

/**
 * @brief destroy kcp control block.
 * 
 * @param kcp The kcp control block.
 */
void kcp_destroy(struct KcpControlBlock *kcp);

/**
 * @brief configure kcp control block.
 * 
 * @param kcp The kcp control block.
 * @param config The kcp configuration.
 *
 * @return int32_t 0 if success, otherwise -1.
 */
int32_t kcp_configure(struct KcpControlBlock *kcp, kcp_config_t *config);

/**
 * @brief send data to peer.
 * @param kcp The kcp control block.
 * @param data The data to send.
 * @param size The size of data.
 * 
 * @return int32_t Return the byte size written to the sending queue.
 */
int32_t kcp_send(struct KcpControlBlock *kcp, const void *data, size_t size);

int32_t kcp_recv(struct KcpControlBlock *kcp, void *data, size_t size);

int32_t kcp_update(struct KcpControlBlock *kcp, uint32_t current);



EXTERN_C_END

#endif // __KCP_PLUS_H__
