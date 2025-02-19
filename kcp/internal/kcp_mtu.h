#ifndef __KCP_MTU_H__
#define __KCP_MTU_H__

#include <stdint.h>
#include <stdbool.h>

#include <event2/event.h>

#include "kcp_def.h"
#include "kcp_net_def.h"
#include "kcp_socket.h"

#define LOCALHOST_MTU       65536
#define ETHERNET_MTU        1500

#define IPV4_HEADER_SIZE    20
#define IPV6_HEADER_SIZE    40
#define UDP_HEADER_SIZE     8

EXTERN_C_BEGIN

int32_t kcp_get_min_mss(bool ipv6);

int32_t kcp_get_mss(bool ipv6);

int32_t kcp_get_localhost_mss(bool ipv6);

typedef void (*on_mtu_probe_completed_t)(kcp_connection_t *kcp_conn, const sockaddr_t *addr, uint32_t addr_len, uint32_t mtu, void *user);
int32_t kcp_mtu_probe(kcp_connection_t *kcp_conn, const sockaddr_t *addr, uint32_t addr_len, uint32_t timeout);

EXTERN_C_END

#endif // __KCP_MTU_H__