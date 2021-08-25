#ifndef __ERRORS_H__
#define __ERRORS_H__

#include <errno.h>

namespace Alias {
typedef enum {
    OK                = 0,
    NO_ERROR          = OK,

    UNKNOWN_ERROR       = (-2147483647-1), // INT32_MIN value

    NO_MEMORY           = -ENOMEM,      /* Out of memory */
    INVALID_OPERATION   = -ENOSYS,      /* Illegal operation */
    INVALID_PARAM       = -EINVAL,      /* Invalid argument */
    NAME_NOT_FOUND      = -ENOENT,      /* No such file or directory */
    PERMISSION_DENIED   = -EPERM,       /* Operation not permitted */
    NO_INIT             = -ENODEV,      /* Uninitialized */
    ALREADY_EXISTS      = -EEXIST,      /* File exists */
    DEAD_OBJECT         = -EPIPE,       /* Object invalid */
    BAD_INDEX           = -EOVERFLOW,   /* Value too large for defined data type */
    NOT_ENOUGH_DATA     = -ENODATA,     /* No data available */
    WOULD_BLOCK         = -EWOULDBLOCK, /* Operation would block */
    TIMED_OUT           = -ETIMEDOUT,   /* Something timed out */
} status_t;

} // namespace Alias

#endif // __ERRORS_H__