#include "log.h"
#include "callstack.h"
#include <sys/syscall.h>

#ifndef gettid
#define gettid() syscall(__NR_gettid)
#endif

#define MSG_BUF_SIZE 512

namespace Jarvis {
static LogManager *gLogManager = nullptr;
static bool gSync = true;
static LogLevel::Level gLevel = LogLevel::DEBUG;

void getLogManager()
{
    if (gLogManager == nullptr) {
        gLogManager = LogManager::getInstance(true, gSync);
    }
}

void InitLog(LogLevel::Level lev, bool sync)
{
    gLevel = lev;
    gSync = sync;

    if (gLogManager == nullptr) {
        gLogManager = LogManager::getInstance(true, sync);
    }
    LogFormat::SetLevel(gLevel);
}

void SetLeval(LogLevel::Level lev)
{
    gLevel = lev;
    LogFormat::SetLevel(gLevel);
}

void addOutputNode(int type)
{
    if (gLogManager != nullptr) {
        gLogManager->addLogWriteToList(type);
    }
}

void delOutputNode(int type)
{
    if (gLogManager != nullptr) {
        gLogManager->delLogWriteFromList(type);
    }
}

void log_write(int level, const char *tag, const char *fmt, ...)
{
    char buf[MSG_BUF_SIZE] = {0};
    LogEvent ev;
    struct timeval tv;
    gettimeofday(&tv, nullptr);

    ev.level = (LogLevel::Level)level;
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
    getLogManager();
    if (gLogManager) {
        std::string msgString = LogFormat::Format(ev);
        if (msgString.size() > 0) {
            gLogManager->WriteLog(msgString);
        }
    }
}

void log_write_assert(int level, const char *expr, const char *tag, const char *fmt, ...)
{
    char buf[MSG_BUF_SIZE] = {0};
    LogEvent ev;
    struct timeval tv;
    gettimeofday(&tv, nullptr);

    ev.level = (LogLevel::Level)level;
    ev.tag = tag;
    ev.time = tv;
    ev.pid = getpid();
    ev.tid = gettid();

    size_t index = snprintf(buf, MSG_BUF_SIZE - 1, "assertion \"%s\" failed. ", expr);
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf + index, MSG_BUF_SIZE - index - 1, fmt, ap);
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
    getLogManager();
    if (gLogManager != nullptr) {
        std::string msgString = LogFormat::Format(ev);
        std::list<LogWrite*> logWriteList = gLogManager->GetLogWrite();
        for (LogManager::LogWriteIt it = logWriteList.begin(); it != logWriteList.end(); ++it) {
            if (*it != nullptr) {
                (*it)->WriteToFile(msgString);
            }
        }
    }
    CallStack cs;
    cs.update(2, 2);
    cs.log("Stack", LogLevel::ERROR);
    exit(0);
}

} // namespace Jarvis