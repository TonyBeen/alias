#include "log_main.h"

static eular::LogManager *lm = nullptr;

namespace eular {
LogManager::LogManager()
{
    pthread_mutex_init(&mListMutex, nullptr);
    mLogWriteList.push_back(new StdoutLogWrite());
}

LogManager::~LogManager()
{
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

void LogManager::WriteLog(LogEvent *event)
{
    for (LogWriteIt it = mLogWriteList.begin(); it != mLogWriteList.end(); ++it) {
        if (*it != nullptr) {
            if ((*it)->type() != LogWrite::STDOUT) {
                event->enableColor = false;
            }
            std::string log = LogFormat::Format(event);
            (*it)->WriteToFile(log);
        }
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

    switch (type) {
        case LogWrite::STDOUT:
            logWrite = new StdoutLogWrite();
            break;
        case LogWrite::FILEOUT:
            logWrite = new FileLogWrite();
            break;
        case LogWrite::CONSOLEOUT:
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

LogManager *LogManager::getInstance()
{
    if (lm == nullptr) {
        lm = new LogManager();
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