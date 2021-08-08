/*************************************************************************
    > File Name: log_format.h
    > Author: hsz
    > Mail:
    > Created Time: 2021年04月12日 星期一 22时16分17秒
 ************************************************************************/

#ifndef __LOG_FORMAT_H__
#define __LOG_FORMAT_H__

#include "log_leavl.h"
#include "log_event.h"
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define LOG_BUF_SIZE (512 + 128)

// 05-24 10:10:10.100 12345 12345 I TAG: msg(\n)
namespace Alias {
class LogFormat {
public:
    LogFormat();
    ~LogFormat();

    LogFormat(const LogFormat&) = delete;
    LogFormat& operator=(const LogFormat&) = delete;

    static std::string Format(const LogEvent *ev);

};

} // namespace Alias

#endif // __LOG_FORMAT_H__