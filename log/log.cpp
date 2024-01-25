#include "log.h"
#include "callstack.h"
#include <sys/syscall.h>
#include <sys/time.h>
#include <assert.h>
#include <time.h>

#ifndef gettid
#define gettid() syscall(__NR_gettid)
#endif

#define MSG_BUF_SIZE (1024)

namespace eular {
static LogManager *gLogManager = nullptr;
static LogLevel::Level gLevel = LogLevel::LEVEL_DEBUG;
static volatile bool gEnableLogoutColor = true;

namespace log {
void getLogManager()
{
    if (gLogManager == nullptr) {
        gLogManager = LogManager::getInstance();
    }
}

void InitLog(LogLevel::Level lev)
{
    gLevel = lev;
    getLogManager();
    LogFormat::SetLevel(gLevel);
}

void SetLevel(LogLevel::Level lev)
{
    gLevel = lev;
    LogFormat::SetLevel(gLevel);
}

void SetPath(const char *path)
{
    getLogManager();
    gLogManager->setPath(path);
}

void EnableLogColor(bool flag)
{
    gEnableLogoutColor = flag;
}

void addOutputNode(int32_t type)
{
    if (gLogManager != nullptr) {
        gLogManager->addLogWriteToList(type);
    }
}

void delOutputNode(int32_t type)
{
    if (gLogManager != nullptr) {
        gLogManager->delLogWriteFromList(type);
    }
}
} // namespace log

void log_write(int32_t level, const char *tag, const char *fmt, ...)
{
    char buf[MSG_BUF_SIZE] = {0};
    char *out = buf;
    uint8_t needFree = 0;
    LogEvent ev;
    struct timeval tv;
    gettimeofday(&tv, nullptr);

    ev.enableColor = gEnableLogoutColor;
    ev.level = (LogLevel::Level)level;
    assert(strlen(tag) < LOG_TAG_SIZE);
    strcpy(ev.tag, tag);
    ev.time = tv;
    ev.pid = getpid();
    ev.tid = gettid();

    va_list ap, tmpArgs;
    va_start(ap, fmt);
    va_copy(tmpArgs, ap);
    int32_t n = vsnprintf(nullptr, 0, fmt, tmpArgs);
    va_end(tmpArgs);

    uint32_t outSize = MSG_BUF_SIZE;
    if (n > MSG_BUF_SIZE) { // 扩充buffer
        outSize = n + 8;
        out = (char *)malloc(outSize);
        if (out == nullptr) {
            out = buf;
            outSize = MSG_BUF_SIZE;
        } else {
            needFree = 1;
        }
        out[outSize - 1] = '\0';
    }
    int32_t formatSize = vsnprintf(out, outSize - 1, fmt, ap);
    va_end(ap);

    if (formatSize < 0) {
        perror("vsnprintf error");
        if (needFree) {
            free(out);
        }
        return;
    }

    size_t len = formatSize;
    if (len && out[len - 1] != '\n') {
        out[len] = '\n';
    }
    ev.msg = out;
    log::getLogManager();
    if (gLogManager) {
        gLogManager->WriteLog(&ev);
    }

    if (needFree) {
        free(out);
    }
}

void log_write_assert(int32_t level, const char *expr, const char *tag, const char *fmt, ...)
{
    char buf[MSG_BUF_SIZE] = {0};
    LogEvent ev;
    struct timeval tv;
    gettimeofday(&tv, nullptr);

    ev.level = (LogLevel::Level)level;
    assert(strlen(tag) < LOG_TAG_SIZE);
    strcpy(ev.tag, tag);
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
    log::getLogManager();
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
    cs.log("Stack", LogLevel::LEVEL_ERROR);
    abort();
}

} // namespace eular