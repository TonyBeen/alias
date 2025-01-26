#ifndef __KCP_NET_UTILS_H__
#define __KCP_NET_UTILS_H__

#include <stdint.h>
#include <kcp_def.h>

EXTERN_C_BEGIN

int32_t set_non_blocking(socket_t fd);
int32_t set_reuse_addr(socket_t fd);
int32_t set_reuse_port(socket_t fd);
int32_t set_dont_fragment(socket_t fd);

EXTERN_C_END

#endif // __KCP_NET_UTILS_H__
