/*************************************************************************
    > File Name: threadpool.cpp
    > Author: hsz
    > Mail:
    > Created Time: Wed 14 Jul 2021 11:29:15 AM CST
 ************************************************************************/

#include "threadpool.h"
#include <utils/utils.h>
#include <utils/Errors.h>
#include <log/log.h>

#define LOG_TAG "ThreadPool"

namespace Jarvis {
static const uint32_t THREAD_NUM_ONCE = 2;

void TaskQueue::addTask(const Task& task, bool insertFront)
{
    AutoLock<Mutex> lock(mQueueMutex);
    if (insertFront) {
        mTaskQueue.insert(mTaskQueue.begin(), task);
    } else {
        mTaskQueue.push_back(task);
    }
}

const Task &TaskQueue::front()
{
    AutoLock<Mutex> lock(mQueueMutex);
    Task t = mTaskQueue.front();
    mTaskQueue.pop_front();
    return t;
}

size_t TaskQueue::taskNumber()
{
    AutoLock<Mutex> lock(mQueueMutex);
    return mTaskQueue.size();
}

ThreadPool::ThreadPool() :
    ThreadPool(MIN_THREAD_NUM, MAX_THREAD_NUM)
{
}

ThreadPool::ThreadPool(size_t minThreadNum, size_t maxThreadNum) :
    mThreadNumMax(MAX(maxThreadNum, minThreadNum)),
    mThreadNumMin(MIN(minThreadNum, maxThreadNum)),
    mWorkThread(nullptr)
{
    mExitNum.store(0);
    mBusyNum.store(0);
    mAliveThreadNum.store(0);
    mShouldExit.store(false, std::memory_order_release);
    mValid = false;

    mTask = new TaskQueue();
    if (mTask == nullptr) {
        LOGE("%s() Create TaskQueue error: no memory", __func__);
        return;
    }
    mManagerThread = new Thread("ThreadPool Manager thread", manager);
    if (mManagerThread == nullptr) {
        LOGE("Create manager thread error: no memory");
        return;
    }
    mManagerThread->setArg(this);
    mManagerThread->run();

    mWorkThread = new Thread[mThreadNumMax];
    if (mWorkThread == nullptr) {
        LOGE("Create worker thread error: no memory");
        return;
    }
    for (size_t i = 0; i < mThreadNumMin; ++i) {
        mWorkThread[i].setWorkFunc(worker);
        mWorkThread[i].setArg(this);
    }
    mValid = true;
}

ThreadPool::~ThreadPool()
{
    mShouldExit.store(true, std::memory_order_release);
    if (mManagerThread) {
        delete mManagerThread;
    }
    if (mWorkThread) {
        delete [] mWorkThread;
    }
    if (mTask) {
        delete mTask;
    }
}

bool ThreadPool::Reinit()
{
    if (mTask == nullptr) {
        mTask = new TaskQueue();
        if (mTask == nullptr) {
            LOGE("%s() Create TaskQueue error: no memory", __func__);
            return false;
        }
    }
    if (mManagerThread == nullptr) {
        mManagerThread = new Thread("ThreadPool Manager thread", manager);
        if (mManagerThread == nullptr) {
            LOGE("%s() Create manager thread error: no memory", __func__);
            return false;
        }
        mManagerThread->setArg(this);
        mManagerThread->run();
    }
    if (mWorkThread == nullptr) {
        mWorkThread = new Thread[mThreadNumMax];
        if (mWorkThread == nullptr) {
            LOGE("Create worker thread error: no memory");
            return false;
        }
        for (size_t i = 0; i < mThreadNumMin; ++i) {
            mWorkThread[i].setWorkFunc(worker);
            mWorkThread[i].setArg(this);
        }
    }
    mValid = true;
    return true;
}

void ThreadPool::startWork()
{
    if (mWorkThread == nullptr) {
        return;
    }
    for (int i = 0; i < mThreadNumMin; ++i) {
        mWorkThread[i].run();
    }
    mAliveThreadNum.store(mThreadNumMin);
}

void ThreadPool::addWork(const Task& td)
{
    AutoLock<Mutex> lock(mPoolMutex);
    if (mTask != nullptr) {
        mTask->addTask(td);
    }
}

int ThreadPool::manager(void *arg)
{
    ThreadPool *pool = static_cast<ThreadPool*>(arg);
    LOG_ASSERT(pool != nullptr, "pool is nullptr");

    while (!pool->mShouldExit.load(std::memory_order_acquire))
    {
        // 管理者线程每1.5s检查一次
        msleep(1500);

        pool->mPoolMutex.lock();
        int queueSize = pool->mTask->taskNumber();
        int busyNum = pool->mBusyNum;
        pool->mPoolMutex.unlock();
        int aliveNum = pool->mAliveThreadNum.load();

        // 任务过多，添加线程
        if (queueSize / 4 > aliveNum && aliveNum < pool->mThreadNumMax) {
            for (size_t i = 0, count = 0; i < pool->mThreadNumMax && THREAD_NUM_ONCE > count; ++i, ++count) {
                if (pool->mWorkThread[i].ThreadStatus() == ThreadBase::THREAD_EXIT) {
                    pool->mWorkThread[i].setWorkFunc(worker);
                    pool->mWorkThread[i].setArg(arg);
                    pool->mWorkThread[i].run();
                    LOGI("Thread %s %d is running", pool->mWorkThread[i].GetThreadName(),
                        pool->mWorkThread[i].getKernalTid());
                    {
                        AutoLock<Mutex> lock(pool->mPoolMutex);
                        pool->mAliveThreadNum++;
                    }
                } else if (pool->mWorkThread[i].ThreadStatus() == ThreadBase::THREAD_WAITING) {
                    pool->mWorkThread[i].setWorkFunc(worker);
                    pool->mWorkThread[i].setArg(arg);
                    pool->mWorkThread[i].StartWork();
                    LOGI("Thread %s %d is running", pool->mWorkThread[i].GetThreadName(),
                        pool->mWorkThread[i].getKernalTid());
                    {
                        AutoLock<Mutex> lock(pool->mPoolMutex);
                        pool->mAliveThreadNum++;
                    }
                }
            }
        }

        // 任务减少，销毁线程
        if (queueSize < aliveNum * 2 && aliveNum > pool->mThreadNumMin) {
            pool->mExitNum = 2;
            for (size_t i = 0; i < 2; ++i) {
                pool->mQueueCond.signal();
            }
            /* for (size_t i = pool->mThreadNumMax - 1, count = 0; 0 < i && THREAD_NUM_ONCE > count; --i) {
                if (pool->mWorkThread[i].ThreadStatus() == ThreadBase::THREAD_RUNNING)  {
                    pool->mWorkThread[i].Interrupt();
                    ++count;
                    {
                        AutoLock<Mutex> lock(pool->mPoolMutex);
                        pool->mAliveThreadNum--;
                    }
                }
            } */
        }
    }
    // 线程退出，回收处理
    for (size_t i = 0; i < pool->mThreadNumMax; ++i) {
        if (pool->mWorkThread[i].ThreadStatus() == ThreadBase::THREAD_EXIT) {
            continue;
        } else {
            pool->mWorkThread[i].Interrupt();
        }
    }
}

int  ThreadPool::worker(void *arg)
{
    ThreadPool *pool = static_cast<ThreadPool *>(arg);
    LOG_ASSERT(pool != nullptr, "pool is nullptr");
    while (!pool->mShouldExit.load(std::memory_order_acquire)) // mShouldExit = true 退出
    {
        // 当任务数量小于线程存活数的2倍时
        while (pool->mTask->taskNumber() < pool->mAliveThreadNum.load() * 2) {
            // 任务队列条件变量
            // 作用：当前任务数量为0，则需阻塞线程，使
            pool->mTask->mQueueMutex.lock();
            pool->mQueueCond.wait(pool->mTask->mQueueMutex);
            if (pool->mExitNum.load() > 0) {
                pool->threadExit();
                return Thread::THREAD_WAITING;
            }
            pool->mTask->mQueueMutex.unlock();
        }

        const Task &t = pool->mTask->front();
        if (t.cb == nullptr) {
            LOGW("task.func is nullptr");
            continue;
        }
        {
            AutoLock<Mutex> lock(pool->mPoolMutex);
            pool->mBusyNum++;
        }
        t.cb(t.data.get());
        {
            AutoLock<Mutex> lock(pool->mPoolMutex);
            pool->mBusyNum--;
        }
    }
    return Thread::THREAD_WAITING;
}

void ThreadPool::threadExit()
{
    LOGI("ThreadPool::threadExit() Thread %d exit\n", gettid());
    // pthread_exit(NULL);
}

} // namespace Jarvis
