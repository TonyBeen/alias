#ifndef __KCP_MTU_H__
#define __KCP_MTU_H__

#include <stdint.h>
#include <stdbool.h>
#include <kcp_def.h>

EXTERN_C_BEGIN

int32_t kcp_get_mtu(bool ipv6);

int32_t kcp_get_localhost_mtu(bool ipv6);

int32_t kcp_mtu_probe(bool ipv6);

EXTERN_C_END

#endif // __KCP_MTU_H__