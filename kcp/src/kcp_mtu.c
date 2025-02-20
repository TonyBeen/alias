#include "kcp_mtu.h"

#define GRE_HEADER_SIZE     24
#define PPPOE_HEADER_SIZE   8
#define MPPE_HEADER_SIZE    2
// packets have been observed in the wild that were fragmented
// with a payload of 1416 for the first fragment
// There are reports of routers that have MTU sizes as small as 1392
#define FUDGE_HEADER_SIZE       36
#define ETHERNET_MTU_V4_MIN     68   // IPv4 minimum MTU
#define ETHERNET_MTU_V6_MIN     1280 // IPv6 minimum MTU

#define UDP_IPV4_OVERHEAD (IPV4_HEADER_SIZE + UDP_HEADER_SIZE)
#define UDP_IPV6_OVERHEAD (IPV6_HEADER_SIZE + UDP_HEADER_SIZE)
#define UDP_TEREDO_OVERHEAD (UDP_IPV4_OVERHEAD + UDP_IPV6_OVERHEAD)

#define UDP_IPV4_MTU (ETHERNET_MTU - IPV4_HEADER_SIZE - UDP_HEADER_SIZE - GRE_HEADER_SIZE - PPPOE_HEADER_SIZE - MPPE_HEADER_SIZE - FUDGE_HEADER_SIZE)
#define UDP_IPV6_MTU (ETHERNET_MTU - IPV6_HEADER_SIZE - UDP_HEADER_SIZE - GRE_HEADER_SIZE - PPPOE_HEADER_SIZE - MPPE_HEADER_SIZE - FUDGE_HEADER_SIZE)
#define UDP_TEREDO_MTU (ETHERNET_MTU_V6_MIN - IPV6_HEADER_SIZE - UDP_HEADER_SIZE)

int32_t kcp_get_min_mss(bool ipv6)
{
    if (ipv6) {
        return ETHERNET_MTU_V6_MIN - IPV6_HEADER_SIZE - UDP_HEADER_SIZE;
    } else {
        return ETHERNET_MTU_V4_MIN - IPV4_HEADER_SIZE - UDP_HEADER_SIZE;
    }
}

int32_t kcp_get_mss(bool ipv6)
{
    if (ipv6) {
        return UDP_TEREDO_MTU;
    } else {
        return UDP_IPV4_MTU;
    }
}

int32_t kcp_get_localhost_mss(bool ipv6)
{
    if (ipv6) {
        return LOCALHOST_MTU - IPV6_HEADER_SIZE - UDP_HEADER_SIZE;
    } else {
        return LOCALHOST_MTU - IPV4_HEADER_SIZE - UDP_HEADER_SIZE;
    }
}

int32_t kcp_mtu_probe(kcp_connection_t *kcp_conn, const sockaddr_t *addr, uint32_t addr_len, uint32_t timeout, uint16_t retry)
{
    
}