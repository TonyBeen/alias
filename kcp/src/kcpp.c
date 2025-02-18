#include "kcpp.h"

#include <event2/event.h>

struct KcpContext {
    socket_t    fd;
    struct event_base *event_base;
};