#include "log_format.h"

namespace Alias {
// 静态成员变量初始化
std::atomic<int> LogFormat::mLevel(LogLevel::DEBUG);

void LogFormat::SetLevel(const LogLevel::Level &lev)
{
    mLevel.exchange((int)lev, std::memory_order_seq_cst);
}

std::string LogFormat::Format(const LogEvent *ev)
{
    if (ev->level < mLevel) {
        return std::string("");
    }

    char buf[LOG_BUF_SIZE] = {0};
    int index = 0;

    // time
    struct tm *pTime = localtime(&(ev->time.tv_sec));
    index = sprintf(buf, "%.2d-%.2d %.2d:%.2d:%.2d.%.3ld",
        pTime->tm_mon, pTime->tm_mday, pTime->tm_hour, pTime->tm_min, pTime->tm_sec, ev->time.tv_usec / 1000);

    // pid tid leval tag: msg
    index += sprintf(buf + index, " %5d %5ld %s %s: %s",
        ev->pid, ev->tid, LogLevel::ToFormatString(ev->level).c_str(), ev->tag, ev->msg);

    return std::string(buf);
}

} // namespace Alias
