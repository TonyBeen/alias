#include "kcp_net_utils.h"
#include "kcp_inc.h"

int32_t set_non_blocking(socket_t fd)
{
#ifdef OS_WINDOWS
    u_long flag = 1;
    return ioctlsocket(fd, FIONBIO, &flag);
#else
    int32_t flag = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, flag | O_NONBLOCK);
#endif
}

int32_t set_reuse_addr(socket_t fd)
{
#ifdef OS_WINDOWS
    BOOL flag = TRUE;
    return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&flag, sizeof(flag));
#else
    int32_t flag = 1;
    return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
#endif
}

int32_t set_reuse_port(socket_t fd)
{
#ifdef OS_WINDOWS
    BOOL flag = TRUE;
    return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&flag, sizeof(flag));
#else
    int32_t flag = 1;
    return setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &flag, sizeof(flag));
#endif
}

int32_t set_dont_fragment(socket_t fd)
{
#ifdef OS_WINDOWS
    BOOL flag = TRUE;
    return setsockopt(fd, IPPROTO_IP, IP_DONTFRAGMENT, (const char *)&flag, sizeof(flag));
#else
    int32_t flag = IP_PMTUDISC_DO;
    return setsockopt(fd, IPPROTO_IP, IP_MTU_DISCOVER, &flag, sizeof(flag));
#endif
}