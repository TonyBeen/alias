/*************************************************************************
    > File Name: log_event.h
    > Author: hsz
    > Desc:
    > Created Time: 2021年04月12日 星期一 22时15Desc
 ************************************************************************/

#ifndef __LOG_EVENT_H__
#define __LOG_EVENT_H__

#include "log_leavl.h"
#include <time.h> 
#include <sys/time.h> 
#include <sys/types.h>

namespace Alias {
struct LogEvent {
    struct timeval  time;       // 时间
    int             pid;        // 进程ID
    pthread_t       tid;        // 线程ID
    LogLeval::Leval leval;      // 日志级别
    const char *    tag;        // 类名
    const char *    msg;        // 日志消息
};

} // namespace Alias
#endif // __LOG_EVENT_H__