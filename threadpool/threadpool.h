/*************************************************************************
    > File Name: threadpool.h
    > Author: hsz
    > Mail:
    > Created Time: Wed 14 Jul 2021 11:29:09 AM CST
 ************************************************************************/

#ifndef __ALIAS_THREAD_POOL_H__
#define __ALIAS_THREAD_POOL_H__

#include <utils/utils.h>
#include <utils/mutex.h>
#include <utils/thread.h>
#include <utils/threaddef.h>
#include <memory>
#include <functional>
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
    Task() : cb(nullptr), data(nullptr) {}
    Task(std::function<int(void *)> f, std::shared_ptr<void *>  d) : cb(f), data(d) {}
    Task(const Task& t)
    {
        this->cb = t.cb;
        this->data = t.data;
    }
    Task &operator=(const Task& t)
    {
        this->cb = t.cb;
        this->data = t.data;
    }
    ~Task() {
        data = nullptr;
        cb = nullptr;
    }

    std::function<int(void *)> cb;
    std::shared_ptr<void *> data;
};

class TaskQueue
{
public:
    TaskQueue() {}
    ~TaskQueue() {}
    void        addTask(const Task& task, bool insertFront = false);
    Task        front();    // 取出头任务
    size_t      taskNumber();

    mutable Mutex mQueueMutex;

private:
    std::list<Task> mTaskQueue;
};

class ThreadPool
{
public:
    DISALLOW_COPY_AND_ASSIGN(ThreadPool);

    ThreadPool();
    ThreadPool(size_t minThreadNum, size_t maxThreadNum);
    ~ThreadPool();

    void startWork();
    void addWork(const Task&);
    void addWork(std::function<int(void *)> f, std::shared_ptr<void *> arg);
    bool isValid() const { return mValid; }
    bool Reinit();

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

    std::atomic<uint32_t> mExitNum;
    std::atomic<uint32_t> mAliveThreadNum;
    std::atomic<uint32_t> mBusyNum;

    Thread*             mManagerThread;
    Thread*             mWorkThread;
    std::atomic_bool    mShouldExit;
    bool                mValid;
};

} // namespace Jarvis
#endif // __ALIAS_THREAD_POOL_H__
