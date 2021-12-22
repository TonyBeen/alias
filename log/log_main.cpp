#include "log_main.h"

static eular::LogManager *lm = nullptr;

namespace eular {
LogManager::LogManager(bool isDetach, bool sync)
{
    pthread_mutex_init(&mListMutex, nullptr);
    mIsDetach = isDetach;
    mIsInterrupt = false;
    mIsSync = sync;
    if (sync == false) {
        pthread_mutex_init(&mExitMutex, nullptr);
        CreateThread(isDetach);
    }

    mLogWriteList.push_back(new StdoutLogWrite());
}

LogManager::~LogManager()
{
    if (mIsSync == false) {
        Interrupt();
        if (!mIsDetach) {
            pthread_join(mLogTid, nullptr);
        }
        pthread_mutex_destroy(&mExitMutex);
        usleep(10000);  // 等待线程退出
    }

    pthread_mutex_destroy(&mListMutex);
    if (mLogWriteList.size() > 0) {
        for (LogWriteIt it = mLogWriteList.begin(); it != mLogWriteList.end();) {
            if (*it != nullptr) {
                delete *it;
                it = mLogWriteList.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void *LogManager::thread(void *arg)
{
    // The thread can be canceled at any time
    // pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, nullptr); 
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

void LogManager::addLogWriteToList(int type)
{
    LogWrite *logWrite = nullptr;

    pthread_mutex_lock(&mListMutex);
    for (LogWriteIt it = mLogWriteList.begin(); it != mLogWriteList.end();) {
        if (*it == nullptr) {           // new 出来的对象为空
            it = mLogWriteList.erase(it);
            continue;
        }
        if ((*it)->type() == type) {    // 如果已存在则直接返回
            goto unlock;
        }
        ++it;
    }

    switch (type)
    {
    case LogWrite::STDOUT:
        logWrite = new StdoutLogWrite();
        break;
    case LogWrite::FILEOUT:
        logWrite = new FileLogWrite();
        break;
    case LogWrite::CONSLOEOUT:
        logWrite = new ConsoleLogWrite();
        break;
    default:
        goto unlock;
    }

    mLogWriteList.push_back(logWrite);
unlock:
    pthread_mutex_unlock(&mListMutex);
}

void LogManager::delLogWriteFromList(int type)
{
    pthread_mutex_lock(&mListMutex);
    for (LogWriteIt it = mLogWriteList.begin(); it != mLogWriteList.end();) {
        if (*it == nullptr) {
            it = mLogWriteList.erase(it);
            continue;
        }
        if ((*it)->type() == type) {
            delete (*it);
            *it = nullptr;      // erase出错导致未删除此处节点时起作用
            mLogWriteList.erase(it);
            break;
        }
        ++it;
    }
    pthread_mutex_unlock(&mListMutex);
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