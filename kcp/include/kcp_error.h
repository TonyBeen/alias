#ifndef __KCP_ERROR_H__
#define __KCP_ERROR_H__

#include <stdint.h>
#include <errno.h>

enum KcpError {
    NO_ERROR = 0,

    // General errors
    UNKNOWN_ERROR       = (-0x7FFFFFFF), // INT32_MIN value

    NO_MEMORY           = -ENOMEM,      /* Out of memory */
    INVALID_OPERATION   = -ENOSYS,      /* Illegal operation */
    INVALID_PARAM       = -EINVAL,      /* Invalid argument */
    NOT_FOUND           = -ENOENT,      /* No such file or directory */
    OPT_NOT_PERMITTED   = -EPERM,       /* Operation not permitted */
    PERMISSION_DENIED   = -EACCES,      /* Permission denied */
    NO_INIT             = -ENODEV,      /* Uninitialized */
    ALREADY_EXISTS      = -EEXIST,      /* File exists */
    DEAD_OBJECT         = -EPIPE,       /* Object invalid */
    BAD_INDEX           = -EOVERFLOW,   /* Value too large for defined data type */
    NOT_ENOUGH_DATA     = -ENODATA,     /* No data available */
    WOULD_BLOCK         = -EWOULDBLOCK, /* Operation would block */
    TIMED_OUT           = -ETIMEDOUT,   /* Something timed out */
    NOT_SUPPORT         = -EOPNOTSUPP,  /* Operation not supported on transport endpoint */
    NO_MORE_ITEM        = -ENOBUFS,     /* No buffer space available */

    // I/O errors
    READ_ERROR          = -10000,
    WRITE_ERROR         = -10001,
    CLOSE_ERROR         = -10002,
    ADD_EVENT_ERROR     = -10003,

    // Network errors
    BIND_ERROR          = -100000,
    LISTEN_ERROR        = -100001,
    ACCEPT_ERROR        = -100002,
    CONNECT_ERROR       = -100003,
};
typedef int32_t kcp_error_t;

#endif // __KCP_ERROR_H__
