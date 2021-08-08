/*************************************************************************
    > File Name: log_main.h
    > Author: hsz
    > Desc: This file is written to handle log events
    > Created Time: 2021年04月12日 星期一 22时15分42秒
 ************************************************************************/

#ifndef __LOG_MAIN_H__
#define __LOG_MAIN_H__

#include "log_leavl.h"
#include "log_write.h"
#include "log_format.h"
#include <utils/mutex.h>
#include <utils/condition.h>
#include <memory>
#include <queue>
#include <pthread.h>

#define LOG_STDOUT
#define MAX_QUEUE_SIZE (1024 * 10)
namespace Alias {
class LogManager : public NonCopyAble, public std::enable_shared_from_this<LogManager> {
public:
    ~LogManager();
    void WriteLog(const std::string& msg);
    static LogManager *getInstance(int threadAttr = 1, bool sync = true);
    static void deleteInstance();
    void Interrupt();
    LogWrite *GetLogWrite() const;

private:
    LogManager(int threadAttr, bool sync = true);
    void CreateThread(int detached);
    static void *thread(void *arg);
    bool ExitThread();

private:
    LogWrite *mLogWrite;
    std::queue<std::string> mQueue;
    Mutex mMutex;
    Condition mCond;
    Mutex mExitMutex;
    pthread_t mLogTid;
    bool mIsDetach;
    bool mIsSync;
    volatile bool mIsInterrupt;
};
} // namespace Alias
#endif // __LOG_MAIN_H__