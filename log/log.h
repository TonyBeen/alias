#ifndef _LOG_H_
#define _LOG_H_

#include "log_main.h"
#include "log_write.h"
#include "log_leavl.h"
#include "log_format.h"
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>

#define MSG_BUF_SIZE 512

#ifndef LOGD
#define LOGD(...) ((void)Alias::log_write(Alias::LogLeval::Leval::DEBUG, LOG_TAG, __VA_ARGS__))
#endif

#ifndef LOGI
#define LOGI(...) ((void)Alias::log_write(Alias::LogLeval::Leval::INFO, LOG_TAG, __VA_ARGS__))
#endif

#ifndef LOGW
#define LOGW(...) ((void)Alias::log_write(Alias::LogLeval::Leval::WARN, LOG_TAG, __VA_ARGS__))
#endif

#ifndef LOGE
#define LOGE(...) ((void)Alias::log_write(Alias::LogLeval::Leval::ERROR, LOG_TAG, __VA_ARGS__))
#endif

#ifndef LOGF
#define LOGF(...) ((void)Alias::log_write(Alias::LogLeval::Leval::FATAL, LOG_TAG, __VA_ARGS__))
#endif

#ifndef LOG_ASSERT
#define LOG_ASSERT(cond, ...) \
    (!(cond) ? ((void)Alias::log_write_assert(Alias::LogLeval::Leval::FATAL, LOG_TAG, __VA_ARGS__)) \
                        : (void)0)
#endif

namespace Alias {
static LogManager *gLogManager = nullptr;

void log_write(int leval, const char *tag, const char *fmt, ...) __attribute__((format(printf, 3, 4)));
void log_write_assert(int leval, const char *tag, const char *fmt, ...) __attribute__((format(printf, 3, 4)));
static void log_writev(const LogEvent *ev);
static void log_write_assertv(const LogEvent *ev);

}

#endif