/*************************************************************************
    > File Name: threadpool.h
    > Author: hsz
    > Mail:
    > Created Time: Wed 14 Jul 2021 11:29:09 AM CST
 ************************************************************************/

#ifndef __ALIAS_THREAD_POOL_H__
#define __ALIAS_THREAD_POOL_H__

#include <stdio.h>
#include <utils/utils.h>
#include <utils/mutex.h>
#include <utils/thread.h>
#include <threaddef.h>
#include <list>
#include <atomic>

#define MIN_THREAD_NUM (4)
#define MAX_THREAD_NUM (8)

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

namespace Jarvis {
struct Task {
    Task() : func(nullptr), data(nullptr) {}
    Task(user_thread_function f, void *d) : func(f), data(d) {}
    ~Task() {
        if (data) {
            delete data;
        }
        data = nullptr;
        func = nullptr;
    }

    user_thread_function func;
    void *data;
};

class TaskQueue
{
public:
    TaskQueue();
    ~TaskQueue();
    void    addTask(const Task& task);
    Task    task();    // 取出头任务
    size_t  taskNumber();

    mutable Mutex mQueueMutex;

private:
    std::list<Task> mQueue;
};

class ThreadPool
{
public:
    DISALLOW_COPY_AND_ASSIGN(ThreadPool);

    ThreadPool();
    ThreadPool(size_t minThreadNum, size_t maxThreadNum);
    ~ThreadPool();

    void    addWork(const Task&);

private:
    static int  manager(void *arg);
    static int  worker(void *arg);
           void threadExit();
private:
    TaskQueue*          mTask;
    Mutex               mPoolMutex;
    Condition           mQueueCond;
    const uint32_t      mThreadNumMax;
    const uint32_t      mThreadNumMin;

    uint32_t            mExitNum;
    uint32_t            mAliveThreadNum;
    uint32_t            mBusyNum;

    Thread*             mManagerThread;
    Thread*             mWorkThread;
    std::atomic_bool    mShouldExit;
    static const size_t THREAD_NUM_ONCE = 2;
};

} // namespace Jarvis
#endif // __ALIAS_THREAD_POOL_H__
