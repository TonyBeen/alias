#ifndef __ERRORS_H__
#define __ERRORS_H__

#include <errno.h>

namespace Alias {
typedef enum {
    OK                = 0,
    NO_ERROR          = OK,

    UNKNOWN_ERROR       = (-2147483647-1), // INT32_MIN value

    NO_MEMORY           = -ENOMEM,
    INVALID_OPERATION   = -ENOSYS,
    INVALID_PARAM       = -EINVAL,
    NAME_NOT_FOUND      = -ENOENT,
    PERMISSION_DENIED   = -EPERM,
    NO_INIT             = -ENODEV,
    ALREADY_EXISTS      = -EEXIST,
    DEAD_OBJECT         = -EPIPE,
    BAD_INDEX           = -EOVERFLOW,
    NOT_ENOUGH_DATA     = -ENODATA,
    WOULD_BLOCK         = -EWOULDBLOCK, 
    TIMED_OUT           = -ETIMEDOUT,
} status_t;

} // namespace Alias

#endif // __ERRORS_H__