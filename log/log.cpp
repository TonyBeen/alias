#include "log.h"
#include <sys/syscall.h>

#ifndef gettid
#define gettid() syscall(__NR_gettid)
#endif

namespace Alias {

void log_write(int leval, const char *tag, const char *fmt, ...)
{
    char buf[MSG_BUF_SIZE] = {0};
    LogEvent ev;
    struct timeval tv;
    gettimeofday(&tv, nullptr);

    ev.leval = (LogLeval::Leval)leval;
    ev.tag = tag;
    ev.time = tv;
    ev.pid = getpid();
    ev.tid = gettid();

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, MSG_BUF_SIZE - 1, fmt, ap);
    va_end(ap);

    size_t len = strlen(buf);
    if (len && buf[len - 1] != '\n') {
        buf[len] = '\n';
    }
    ev.msg = buf;
    log_writev(&ev);
}

static void log_writev(const LogEvent *ev)
{
    if (gLogManager == nullptr) {
        gLogManager = LogManager::getInstance();
    }
    std::string msgString = LogFormat::Format(ev);
    gLogManager->WriteLog(msgString);
}

void log_write_assert(int leval, const char *tag, const char *fmt, ...)
{
    char buf[MSG_BUF_SIZE] = {0};
    LogEvent ev;
    struct timeval tv;
    gettimeofday(&tv, nullptr);

    ev.leval = (LogLeval::Leval)leval;
    ev.tag = tag;
    ev.time = tv;
    ev.pid = getpid();
    ev.tid = gettid();

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, MSG_BUF_SIZE - 1, fmt, ap);
    va_end(ap);

    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] != '\n') {
        buf[len] = '\n';
    }
    ev.msg = buf;
    log_write_assertv(&ev);
}

void log_write_assertv(const LogEvent *ev)
{
    if (gLogManager == nullptr) {
        gLogManager = LogManager::getInstance();
    }
    std::string msgString = LogFormat::Format(ev);
    gLogManager->Interrupt();
    gLogManager->GetLogWrite()->WriteToFile(msgString);

    abort();
}

} // namespace Alias