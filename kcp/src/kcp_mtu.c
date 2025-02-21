#include "kcp_mtu.h"

#include <event2/event.h>

#include "kcp_error.h"

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

typedef struct KcpMtuProbeCB {
    struct event*           ev_timer;
    kcp_connection_t*       kcp_conn;
    uint32_t                mtu_best;
    uint32_t                mtu_lbound;
    uint32_t                mtu_ubound;
    uint32_t                timeout;
    uint16_t                retry;
} kcp_mtu_probe_cb_t;

static void kcp_mtu_probe_cb(evutil_socket_t fd, short event, void *arg)
{
    kcp_mtu_probe_cb_t *probe_cb = (kcp_mtu_probe_cb_t *)arg;
    if (probe_cb == NULL) {
        return;
    }

    if (probe_cb->retry == 0) {
        evtimer_del(probe_cb->ev_timer);
        evtimer_free(probe_cb->ev_timer);
        // TODO 回调探测失败
        free(probe_cb);
        return;
    }

    probe_cb->retry--;

    // TODO send probe packet
    // kcp_send_probe_packet(probe_cb->kcp_conn);

    evtimer_add(probe_cb->ev_timer, probe_cb->timeout);
}

int32_t kcp_mtu_probe(kcp_connection_t *kcp_conn, uint32_t timeout, uint16_t retry)
{
    if (kcp_conn == NULL || kcp_conn->on_probe_completed == NULL || timeout == 0) {
        return INVALID_PARAM;
    }

    kcp_mtu_probe_cb_t *probe_cb = (kcp_mtu_probe_cb_t *)malloc(sizeof(kcp_mtu_probe_cb_t));
    if (probe_cb == NULL) {
        return NO_MEMORY;
    }

    probe_cb->kcp_conn = kcp_conn;
    probe_cb->mtu_best = ETHERNET_MTU_V4_MIN;
    probe_cb->mtu_lbound = ETHERNET_MTU_V4_MIN;
    probe_cb->mtu_ubound = LOCALHOST_MTU;
    probe_cb->timeout = timeout;
    probe_cb->retry = retry;
    probe_cb->ev_timer = evtimer_new(kcp_conn->kcp_ctx->event_loop, kcp_mtu_probe_cb, probe_cb);
    if (probe_cb->ev_timer == NULL) {
        free(probe_cb);
        return NO_MEMORY;
    }
    kcp_conn->probe_user_data = probe_cb;

    evtimer_add(probe_cb->ev_timer, timeout);
    return NO_ERROR;
}

int32_t kcp_mtu_probe_received(kcp_connection_t *kcp_conn, void *arg)
{

}