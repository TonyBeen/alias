/*************************************************************************
    > File Name: timer.h
    > Author: hsz
    > Mail:
    > Created Time: Thu 16 Sep 2021 02:32:45 PM CST
 ************************************************************************/

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
class Timer
{
public:
    typedef std::shared_ptr<Timer> sp;
    typedef std::function<int(void *)> CallBack;
    Timer(uint64_t ms, CallBack cb, bool recycle = false);
    ~Timer();
    void concel();
    void refresh();
    /**
     * @param ms        下一次执行时间(相对时间：当前时间戳 + ms为下一次执行时间)
     * @param recycle   是否循环
     */
    void reset(uint64_t ms, bool recycle);

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
    uint64_t    mTime;          // 下一次执行时间
    uint64_t    mRecycleTime;   // 循环时间ms
    CallBack    mCb;            // 回调函数
    bool        mRecycle;       // 是否循环
};

class TimerManager
{
public:
    TimerManager();
    ~TimerManager();

    void addTimer(uint64_t ms, Timer::CallBack cb, bool recycle = false);

private:
    RWMutex mRWMutex;
    std::set<Timer::sp, Timer::Comparator>  mTimers;        // 定时器集合

};

} // namespace Jarvis

#endif // __TIMER_H__