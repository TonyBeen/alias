#include "kcp_mtu.h"

#define ETHERNET_MTU 1500
#define IPV4_HEADER_SIZE 20
#define IPV6_HEADER_SIZE 40
#define UDP_HEADER_SIZE 8
#define GRE_HEADER_SIZE 24
#define PPPOE_HEADER_SIZE 8
#define MPPE_HEADER_SIZE 2
// packets have been observed in the wild that were fragmented
// with a payload of 1416 for the first fragment
// There are reports of routers that have MTU sizes as small as 1392
#define FUDGE_HEADER_SIZE 36
#define TEREDO_MTU 1280

#define UDP_IPV4_OVERHEAD (IPV4_HEADER_SIZE + UDP_HEADER_SIZE)
#define UDP_IPV6_OVERHEAD (IPV6_HEADER_SIZE + UDP_HEADER_SIZE)
#define UDP_TEREDO_OVERHEAD (UDP_IPV4_OVERHEAD + UDP_IPV6_OVERHEAD)

#define UDP_IPV4_MTU (ETHERNET_MTU - IPV4_HEADER_SIZE - UDP_HEADER_SIZE - GRE_HEADER_SIZE - PPPOE_HEADER_SIZE - MPPE_HEADER_SIZE - FUDGE_HEADER_SIZE)
#define UDP_IPV6_MTU (ETHERNET_MTU - IPV6_HEADER_SIZE - UDP_HEADER_SIZE - GRE_HEADER_SIZE - PPPOE_HEADER_SIZE - MPPE_HEADER_SIZE - FUDGE_HEADER_SIZE)
#define UDP_TEREDO_MTU (TEREDO_MTU - IPV6_HEADER_SIZE - UDP_HEADER_SIZE)

int32_t kcp_get_mtu(bool ipv6)
{
    if (ipv6) {
        return UDP_TEREDO_MTU;
    } else {
        return UDP_IPV4_MTU;
    }
}

int32_t kcp_mtu_probe(bool ipv6)
{
    if (ipv6) {
        return (ETHERNET_MTU - IPV6_HEADER_SIZE - UDP_HEADER_SIZE);
    } else {
        return (ETHERNET_MTU - IPV4_HEADER_SIZE - UDP_HEADER_SIZE);
    }
}