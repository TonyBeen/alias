#include "log_main.h"

static Alias::LogManager *lm = nullptr;

namespace Alias {
LogManager::LogManager(int threadAttr, bool sync)
{
    mIsDetach = threadAttr;
    mIsInterrupt = false;
    mIsSync = sync;
    if (sync == false) {
        CreateThread(threadAttr);
    }

#if defined(LOG_STDOUT)
    mLogWrite = new StdoutLogWrite();
#else
    mLogWrite = new FileLogWrite();
#endif
}

LogManager::~LogManager()
{
    if (mIsSync == false) {
        Interrupt();
        if (!mIsDetach) {
            pthread_join(mLogTid, nullptr);
        }
        printf("%s()\n", __func__);
        usleep(10000);  // 等待线程退出
    }
    if (mLogWrite != nullptr) {
        delete mLogWrite;
        mLogWrite = nullptr;
    }
}

void *LogManager::thread(void *arg)
{
    // pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, nullptr); // ->The thread can be canceled at any time
    LogManager *logManager = (LogManager *)arg;
    while (logManager->ExitThread() == false) {
        std::string msg;
        {
            AutoLock<Mutex> _l(logManager->mMutex);
            while (logManager->mQueue.size() == 0) {
                logManager->mCond.wait(logManager->mMutex);
            }
            msg = logManager->mQueue.front();
            logManager->mQueue.pop();
        }
        logManager->mLogWrite->WriteToFile(msg);
    }
    printf("log write thread exit\n");
}

void LogManager::CreateThread(int detached)
{
    if (detached) {
        pthread_attr_t detach;
        pthread_attr_init(&detach);
        pthread_attr_setdetachstate(&detach, PTHREAD_CREATE_DETACHED);
        pthread_create(&mLogTid, &detach, thread, this);
        pthread_attr_destroy(&detach);
    } else {
        pthread_create(&mLogTid, nullptr, thread, this);
    }
}

void LogManager::Interrupt()
{
    AutoLock<Mutex> lock(mExitMutex);
    mIsInterrupt = true;
}

bool LogManager::ExitThread()
{
    AutoLock<Mutex> lock(mExitMutex);
    return mIsInterrupt;
}

void LogManager::WriteLog(const std::string& msg)
{
    if (mIsSync == false) {
        if (ExitThread()) {
            return;
        }
        AutoLock<Mutex> lock(mMutex);
        if (mQueue.size() >= MAX_QUEUE_SIZE) {
            return;
        }
        mQueue.push(msg);
        if (mQueue.size() == 2) {
            mCond.broadcast();
        }
    } else {
        mLogWrite->WriteToFile(msg);
    }
}

LogWrite *LogManager::GetLogWrite() const
{
    return mLogWrite;
}

LogManager *LogManager::getInstance(int threadAttr, bool sync)
{
    if (lm == nullptr) {
        lm = new LogManager(threadAttr, sync);
    }
    return lm;
}

void LogManager::deleteInstance()
{
    if (lm != nullptr) {
        delete lm;
        lm = nullptr;
    }
}

}