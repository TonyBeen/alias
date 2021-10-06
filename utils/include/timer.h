/*************************************************************************
    > File Name: timer.h
    > Author: hsz
    > Mail:
    > Created Time: Thu 16 Sep 2021 02:32:45 PM CST
 ************************************************************************/

/**
 *  基于epoll_wait实现的定时器
 *  epoll + 线程死循环
 */

#ifndef __TIMER_H__
#define __TIMER_H__

#include "mutex.h"
#include <stdint.h>
#include <set>
#include <memory>
#include <functional>

using std::set;
using std::function;
using std::shared_ptr;

namespace Jarvis {
class TimerManager;
class Timer : public std::enable_shared_from_this<Timer>
{
public:
    typedef std::shared_ptr<Timer> sp;
    typedef std::function<int(void *)> CallBack;
    Timer();
    Timer(uint64_t ms, CallBack cb, uint32_t recycle);
    ~Timer();

    void setNextTime(uint64_t timeMs) { mTime = timeMs; }
    void setCallback(CallBack cb) { mCb = cb; }
    void setRecycleTime(uint64_t ms) { mRecycleTime = ms; }

    void concel();
    void refresh();
    void setArg(std::shared_ptr<void *> arg) { mArg = arg; }

    /**
     * @param ms        下一次执行时间(相对时间：当前时间戳 + ms为下一次执行时间)
     * @param recycle   是否循环
     */
    void reset(uint64_t ms, CallBack cb, uint32_t recycle);

    static uint64_t getCurrentTime();

private:
    friend class TimerManager;
    struct Comparator {
        // 传给set的比较器，从小到大排序
        bool operator()(const Timer::sp &l, const Timer::sp &r) {
            if (l == nullptr && r == nullptr) {
                return false;
            }
            if (l == nullptr) {
                return true;
            }
            if (r == nullptr) {
                return false;
            }
            if (l->mTime == r->mTime) { // 时间相同，比较地址
                return l.get() < r.get();
            }
            return l->mTime < r->mTime;
        }
    };
private:
    uint64_t    mTime;          // (绝对时间)下一次执行时间
    uint64_t    mRecycleTime;   // 循环时间ms
    CallBack    mCb;            // 回调函数
    std::shared_ptr<void *> mArg;   // 函数参数
};

class TimerManager
{
public:
    typedef std::set<Timer::sp, Timer::Comparator>::iterator TimerIterator;
    TimerManager();
    ~TimerManager();

    const Timer::sp &getNearTimer() { return *(mTimers.begin()); }
    TimerIterator addTimer(uint64_t ms, Timer::CallBack cb,
        std::shared_ptr<void *> arg = nullptr, uint32_t recycle = 0);
    void delTimer(TimerIterator it);

private:
    RWMutex mRWMutex;
    std::set<Timer::sp, Timer::Comparator>  mTimers;        // 定时器集合
};

} // namespace Jarvis

#endif // __TIMER_H__