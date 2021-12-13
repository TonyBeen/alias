/*************************************************************************
    > File Name: timer.cpp
    > Author: hsz
    > Mail:
    > Created Time: Thu 16 Sep 2021 02:33:01 PM CST
 ************************************************************************/

// #define DEBUG

#include "timer.h"
#include "exception.h"
#include "Errors.h"
#include "debug.h"
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <atomic>

namespace Jarvis {
static std::atomic<uint64_t> gTimerCount = {0};
static const uint32_t gMaxEpollEvents = 2;

Timer::Timer(uint64_t ms, CallBack cb, uint32_t recycle) :
    mCb(cb),
    mArg(nullptr),
    mRecycleTime(recycle)
{
    mUniqueId = ++gTimerCount;
    mTime = getCurrentTime() + ms;
}

Timer::Timer(const Timer& timer) :
    mUniqueId(timer.mUniqueId),
    mTime(timer.mTime),
    mRecycleTime(timer.mRecycleTime),
    mCb(timer.mCb),
    mArg(timer.mArg)
{

}

Timer::~Timer()
{

}

Timer &Timer::operator=(const Timer& timer)
{
    mUniqueId = timer.mUniqueId;
    mTime = timer.mTime;
    mRecycleTime = timer.mRecycleTime;
    mCb = timer.mCb;
    mArg = timer.mArg;
}

/**
 * @brief 取消执行回调
 */
void Timer::cancel()
{
    mCb = nullptr;
}

/**
 * @brief 刷新下次执行时间，仅当设置循环生效
 */
void Timer::refresh()
{
    if (mRecycleTime > 0) {
        mTime = getCurrentTime() + mRecycleTime;
    }
}

/**
 * @brief 重新设置执行时间，回调，循环执行时间
 */
void Timer::reset(uint64_t ms, CallBack cb, uint32_t recycle)
{
    mTime = getCurrentTime() + ms;
    mCb = cb;
    mRecycleTime = recycle;
}

/**
 * @brief 获取当前系统的绝对时间
 * @return CLOCK_MONOTONIC 获取到的绝对时间，单位毫秒
 */
uint64_t Timer::getCurrentTime()
{
    timespec curTime;
    clock_gettime(CLOCK_MONOTONIC, &curTime);   // 获取系统运行时间，如果使用realtime，可能存在修改系统时间的进程，导致定时器出问题
    uint64_t time = curTime.tv_sec * 1000 + curTime.tv_nsec / 1000 / 1000;
    return time;
}

TimerManager::TimerManager() :
    mEpollFd(-1),
    ThreadBase("timer thread", true),
    mSignal(0)
{

}

TimerManager::~TimerManager()
{
    if (mEpollFd > 0) {
        close(mEpollFd);
    }
    mRWMutex.wlock();
    for (auto& it : mTimers) {
        if (it != nullptr) {
            delete it;
        }
    }
    mTimers.clear();
    mRWMutex.unlock();
}

int TimerManager::StartTimer(bool useCallerThread)
{
    mEpollFd = epoll_create(gMaxEpollEvents);
    if (mEpollFd < 0) {
        return UNKNOWN_ERROR;
    }
    epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = 0;
    epoll_ctl(mEpollFd, EPOLL_CTL_ADD, 0, &ev);
    LOG("epoll fd = %d\n", mEpollFd);

    if (useCallerThread) {
        return threadWorkFunction(this);
    }
    return run();
}

/**
 * @brief   添加一个定时器
 * @param ms 延迟的时间，单位毫秒
 * @param cb 回调函数
 * @param arg 回调函数的入参
 * @param recycle 循环调用的时间，第一次调用为ms时，下一次为recycle毫秒后，循环
 * @return 返回定时器唯一ID，方便删除
 */
uint64_t TimerManager::addTimer(
    uint64_t ms, Timer::CallBack cb, std::shared_ptr<void *> arg, uint32_t recycle)
{
    WRAutoLock<RWMutex> wrLock(mRWMutex);
    Timer *timer = new Timer(ms, cb, recycle);
    if (timer == nullptr) {
        return 0;
    }
    timer->setArg(arg);
    auto pair = mTimers.insert(timer);
    if (pair.second) {
        if (mTimers.size() == 1) {
            mSignal.post();
        }
        return (*pair.first)->mUniqueId;
    }

    delete timer;
    return 0;
}

void TimerManager::addTimer(Timer *timer)
{
    if (timer == nullptr) {
        return;
    }
    WRAutoLock<RWMutex> lock(mRWMutex);
    mTimers.insert(timer);
}

bool TimerManager::delTimer(uint64_t uniqueId)
{
    WRAutoLock<RWMutex> wrLock(mRWMutex);
    bool flag = false;
    for (auto it = mTimers.begin(); it != mTimers.end();) {
        if ((*it)->mUniqueId == uniqueId) {
            mTimers.erase(it);
            flag = true;
            break;
        }
        ++it;
    }
    return flag;
}

void TimerManager::ListExpireTimer()
{
    uint64_t currTimeMs = Timer::getCurrentTime();
    WRAutoLock<RWMutex> wrLock(mRWMutex);
    for (TimerManager::TimerIterator it = mTimers.begin(); it != mTimers.end(); /**/) {
        if ((*it)->mTime < currTimeMs) {
            mExpireTimerVec.push_back(*it);
            it = mTimers.erase(it);
        } else {
            ++it;
        }
    }
    LOG("mExpireTimerVec size = %zu\n", mExpireTimerVec.size());
}

int TimerManager::threadWorkFunction(void *arg)
{
    TimerManager::TimerIterator it;
    epoll_event ev[gMaxEpollEvents];
    int n = 0;
    struct timespec sleepTime;
    sleepTime.tv_nsec = 1000000;
    sleepTime.tv_sec = 0;

    LOG("timer thread loop begin\n");
    while (true) {
        {
            RDAutoLock<RWMutex> lock(mRWMutex);
            LOG("timers size %zu\n", mTimers.size());
            if (mTimers.size() == 0) {
                LOG("timer wait");
                mRWMutex.unlock();
                mSignal.wait();
            }
            mRWMutex.rlock();
            it = mTimers.begin();
        }

        int nextTime = (*it)->mTime - Timer::getCurrentTime();
        LOG("nextTime = %d\n", nextTime);
        if (nextTime > 0) {
            n = epoll_wait(mEpollFd, ev, gMaxEpollEvents, nextTime);
        }
        if (n == 0 || nextTime < 0) {
            ListExpireTimer();
            for (auto &vecIt : mExpireTimerVec) {
                if (vecIt->mCb != nullptr) {
                    try {
                        vecIt->mCb(vecIt->mArg.get());
                    } catch (const Exception &e) {
                        LOG("%s\n", e.what());
                    } catch (...) {

                    }
                }
                if (vecIt->mRecycleTime > 0) {
                    vecIt->mTime += vecIt->mRecycleTime;
                    addTimer(vecIt);
                } else {
                    delete vecIt;
                }
            }
            mExpireTimerVec.clear();
        }
        if (n < 0) {
            LOG("epoll_wait error. [%d, %s]", errno, strerror(errno));
            break;
        }
    }

    return ThreadBase::THREAD_WAITING;
}

} // namespace Jarvis
