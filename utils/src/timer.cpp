/*************************************************************************
    > File Name: timer.cpp
    > Author: hsz
    > Mail:
    > Created Time: Thu 16 Sep 2021 02:33:01 PM CST
 ************************************************************************/

#include "timer.h"
#include "exception.h"
#include "Errors.h"
#include <time.h>
#include <unistd.h>
#include <atomic>

namespace Jarvis {
static std::atomic<uint64_t> gTimerCount = {0};

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
void Timer::concel()
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
    uint64_t time = curTime.tv_sec * 1000 + curTime.tv_nsec / 1000;
    return time;
}

TimerManager::TimerManager() :
    mEpollFd(-1)
{
    mThread.setWorkFunc(timer_thread_loop);
    mThread.setArg(this);
    mThread.setThreadName("Timer thread");
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
    mThread.Interrupt();
}

int TimerManager::StartTimer(bool useCallerThread)
{
    mEpollFd = epoll_create(1);
    if (mEpollFd < 0) {
        return UNKNOWN_ERROR;
    }
    epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = 0;
    epoll_ctl(mEpollFd, EPOLL_CTL_ADD, 0, &ev);
    printf("epoll fd = %d\n", mEpollFd);
    if (useCallerThread) {
        return timer_thread_loop(this);
    }
    return mThread.run();
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
        return (*pair.first)->mUniqueId;
    }

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

int TimerManager::timer_thread_loop(void *arg)
{
    TimerManager *tm = nullptr;
    if (arg == nullptr) {
        return Thread::THREAD_EXIT;
    }

    tm = (TimerManager *)arg;
    TimerManager::TimerIterator it;
    epoll_event ev;
    int n = -1;

    timespec sleepTime;
    sleepTime.tv_nsec = 1000000;
    sleepTime.tv_sec = 0;
    printf("timer thread loop begin\n");
    while (1) {
        {
            WRAutoLock<RWMutex> lock(tm->mRWMutex);
            printf("1 timers size %zu\n", tm->mTimers.size());
            it = tm->mTimers.begin();
            if (it == tm->mTimers.end()) {
                break;
            }
            printf("unique id = %lu\n", (*it)->mUniqueId);
        }

        int nextTime = (*it)->mTime - Timer::getCurrentTime();
        if (nextTime > 0) {
            n = epoll_wait(tm->mEpollFd, &ev, 1, nextTime);
        }

        // TODO: epoll_wait返回后，启用线程还是协程处理？
        // 另外，当有多个时间相同时如何做到在短时间内处理，线城池还是协程池
        if (n == 0 || nextTime < 0) {
            try {
                (*it)->mCb((*it)->mArg.get());
            } catch (const std::exception& e) {
                printf("%s", e.what());
            } catch (...) {
                printf("%lu ms, unknow error.", (*it)->mTime);
            }
        }
        printf("func execute over\n");
        {
            static Timer *ptr;
            ptr = *it;
            tm->delTimer(ptr->mUniqueId);
            if (ptr->mRecycleTime > 0) {
                tm->addTimer(ptr);
            }
            printf("2 timers size %zu\n", tm->mTimers.size());
        }
    }
    printf("timer thread wait");
    return Thread::THREAD_WAITING;
    // 线程处于等待状态，需在addTimer调用startWork。
}

} // namespace Jarvis
