#include "log_format.h"

namespace Alias {
LogLevel::Level LogFormat::mLevel = LogLevel::INFO;

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
