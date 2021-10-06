/*************************************************************************
    > File Name: timer.cpp
    > Author: hsz
    > Mail:
    > Created Time: Thu 16 Sep 2021 02:33:01 PM CST
 ************************************************************************/

#include "timer.h"
#include <time.h>

namespace Jarvis {
Timer::Timer(uint64_t ms, CallBack cb, uint32_t recycle) :
    mCb(cb),
    mArg(nullptr),
    mRecycleTime(recycle)
{
    mTime = getCurrentTime() + ms;
}

Timer::~Timer()
{

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
 * @return 绝对时间，单位毫秒
 */
uint64_t Timer::getCurrentTime()
{
    timespec curTime;
    clock_gettime(CLOCK_REALTIME, &curTime);
    uint64_t time = curTime.tv_sec * 1000 + curTime.tv_nsec / 1000;
    return time;
}

TimerManager::TimerManager()
{

}

TimerManager::~TimerManager()
{

}

TimerManager::TimerIterator TimerManager::addTimer(
    uint64_t ms, Timer::CallBack cb, std::shared_ptr<void *> arg, uint32_t recycle)
{
    Timer::sp timer(new Timer(ms, cb, recycle));
    if (timer == nullptr) {
        return mTimers.end();
    }
    timer->setArg(arg);
    auto pair = mTimers.insert(timer);
    if (pair.second) {
        return pair.first;
    }

    return mTimers.end();
}

void TimerManager::delTimer(TimerManager::TimerIterator it)
{
    if (it == mTimers.end()) {
        return;
    }
    mTimers.erase(it);
}

} // namespace Jarvis
