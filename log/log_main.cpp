#include "log_main.h"

static Alias::LogManager *lm = nullptr;

namespace Alias {
LogManager::LogManager(bool isDetach, bool sync)
{
    mIsDetach = isDetach;
    mIsInterrupt = false;
    mIsSync = sync;
    if (sync == false) {
        CreateThread(isDetach);
    }

    mLogWriteList.push_back(new StdoutLogWrite());
    mLogWriteList.push_back(new FileLogWrite());
    mLogWriteList.push_back(new ConsoleLogWrite());
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

    if (mLogWriteList.size() > 0) {
        for (LogWriteIt it = mLogWriteList.begin(); it != mLogWriteList.end(); ++it) {
            delete *it;
            mLogWriteList.erase(it);
        }
    }
}

void *LogManager::thread(void *arg)
{
    // pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, nullptr); // ->The thread can be canceled at any time
    LogManager *logManager = (LogManager *)arg;
    while (logManager->ExitThread() == false) {
        std::string msg;
        {
            pthread_mutex_lock(&logManager->mMutex);
            while (logManager->mQueue.size() == 0) {
                pthread_cond_wait(&logManager->mCond, &logManager->mMutex);
            }
            msg = logManager->mQueue.front();
            logManager->mQueue.pop();
            pthread_mutex_unlock(&logManager->mMutex);
        }
        for (LogWriteIt it = logManager->mLogWriteList.begin(); it != logManager->mLogWriteList.end(); ++it) {
            if (*it != nullptr) {
                (*it)->WriteToFile(msg);
            }
        }
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
    pthread_mutex_lock(&mExitMutex);
    mIsInterrupt = true;
    pthread_mutex_unlock(&mExitMutex);
}

bool LogManager::ExitThread()
{
    pthread_mutex_lock(&mExitMutex);
    return mIsInterrupt;
    pthread_mutex_unlock(&mExitMutex);
}

void LogManager::WriteLog(const std::string& msg)
{
    if (mIsSync) {
        for (LogWriteIt it = mLogWriteList.begin(); it != mLogWriteList.end(); ++it) {
            if (*it != nullptr) {
                (*it)->WriteToFile(msg);
            }
        }
    }
    if (mIsSync == false) {
        if (ExitThread()) {
            return;
        }
        pthread_mutex_lock(&mMutex);
        if (mQueue.size() >= MAX_QUEUE_SIZE) {
            return;
        }
        mQueue.push(msg);
        if (mQueue.size() == 2) {
            pthread_cond_broadcast(&mCond);
        }
        pthread_mutex_unlock(&mMutex);
    }
}

const std::list<LogWrite *> &LogManager::GetLogWrite() const
{
    return mLogWriteList;
}

LogManager *LogManager::getInstance(bool isDetach, bool sync)
{
    if (lm == nullptr) {
        lm = new LogManager(isDetach, sync);
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