/*************************************************************************
    > File Name: log_main.h
    > Author: hsz
    > Desc: This file is written to handle log events
    > Created Time: 2021年04月12日 星期一 22时15分42秒
 ************************************************************************/

#ifndef __LOG_MAIN_H__
#define __LOG_MAIN_H__

#include "log_level.h"
#include "log_write.h"
#include "log_format.h"
#include <memory>
#include <queue>
#include <pthread.h>

#define MAX_QUEUE_SIZE (1024 * 10)

namespace Alias {
class LogManager : public std::enable_shared_from_this<LogManager> {
public:
    ~LogManager();
    LogManager(const LogManager&) = delete;
    LogManager& operator=(const LogManager&) = delete;

    void WriteLog(const std::string& msg);
    static LogManager *getInstance(bool isDetach = true, bool sync = true, bool isStdout = true);
    static void deleteInstance();
    void Interrupt();
    LogWrite *GetLogWrite() const;

private:
    LogManager() = default;
    LogManager(bool, bool, bool);
    void CreateThread(int detached);
    static void *thread(void *arg);
    bool ExitThread();

private:
    LogWrite *mLogWrite;
    std::queue<std::string> mQueue;
    pthread_mutex_t         mMutex;
    pthread_cond_t          mCond;
    pthread_mutex_t         mExitMutex;
    pthread_t mLogTid;
    bool mIsDetach;
    bool mIsSync;
    volatile bool mIsInterrupt;
};
} // namespace Alias
#endif // __LOG_MAIN_H__