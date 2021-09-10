#ifndef _LOG_H_
#define _LOG_H_

#include "log_main.h"
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>

#ifndef LOGD
#define LOGD(...) ((void)Alias::log_write(Alias::LogLevel::Level::DEBUG, LOG_TAG, __VA_ARGS__))
#endif

#ifndef LOGI
#define LOGI(...) ((void)Alias::log_write(Alias::LogLevel::Level::INFO, LOG_TAG, __VA_ARGS__))
#endif

#ifndef LOGW
#define LOGW(...) ((void)Alias::log_write(Alias::LogLevel::Level::WARN, LOG_TAG, __VA_ARGS__))
#endif

#ifndef LOGE
#define LOGE(...) ((void)Alias::log_write(Alias::LogLevel::Level::ERROR, LOG_TAG, __VA_ARGS__))
#endif

#ifndef LOGF
#define LOGF(...) ((void)Alias::log_write(Alias::LogLevel::Level::FATAL, LOG_TAG, __VA_ARGS__))
#endif

#ifndef LOG_ASSERT
#define LOG_ASSERT(cond, ...) \
    (!(cond) ? ((void)Alias::log_write_assert(Alias::LogLevel::Level::FATAL, #cond, LOG_TAG, __VA_ARGS__)) : (void)0)
#endif

namespace Alias {
// interface
void InitLog(LogLevel::Level lev = LogLevel::DEBUG, bool sync = true);
void SetLevel(LogLevel::Level lev);

void log_write(int level, const char *tag, const char *fmt, ...) __attribute__((format(printf, 3, 4)));
void log_write_assert(int level, const char *expr, const char *tag, const char *fmt, ...) __attribute__((format(printf, 4, 5)));
static void log_writev(const LogEvent *ev);
static void log_write_assertv(const LogEvent *ev);

}

#endif