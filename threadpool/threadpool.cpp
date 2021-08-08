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

namespace Alias {
void TaskQueue::addTask(const Task& task)
{
    AutoLock<Mutex> lock(mQueueMutex);
    mQueue.push_back(task);
}

Task TaskQueue::task()
{
    AutoLock<Mutex> lock(mQueueMutex);
    Task t = mQueue.front();
    mQueue.pop_front();
    return t;
}

size_t TaskQueue::taskNumber()
{
    AutoLock<Mutex> lock(mQueueMutex);
    return mQueue.size();
}

ThreadPool::ThreadPool() :
    ThreadPool(MIN_THREAD_NUM, MAX_THREAD_NUM)
{
}

ThreadPool::ThreadPool(size_t minThreadNum, size_t maxThreadNum) :
    mThreadNumMax(MAX(maxThreadNum, minThreadNum)),
    mThreadNumMin(MIN(minThreadNum, maxThreadNum)),
    mExitNum(0),
    mBusyNum(0),
    mAliveThreadNum(0),
    mWorkThread(nullptr),
    mShouldExit(false)
{
    mTask = new TaskQueue();
    if (mTask == nullptr) {
        LOGE("Create TaskQueue error: no memory");
        return;
    }
    mManagerThread = new Thread("ThreadPool Manager thread", manager);
    if (mManagerThread == nullptr) {
        LOGE("Create manager thread error: no memory");
        return;
    }
    mManagerThread->setArg(this);

    mWorkThread = new Thread[mThreadNumMax];
    if (mWorkThread == nullptr) {
        LOGE("Create worker thread error: no memory");
        delete mManagerThread;
        return;
    }
    for (size_t i = 0; i < mThreadNumMin; ++i) {
        mWorkThread[i].setWorkFunc(worker);
        mWorkThread[i].setArg(this);
        mWorkThread[i].run();
    }
    mManagerThread->run();
}

ThreadPool::~ThreadPool()
{
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

void ThreadPool::addWork(const Task& td)
{
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
        int aliveNum = pool->mAliveThreadNum;
        pool->mPoolMutex.unlock();

        // 任务过多，添加线程
        if (queueSize / 4 > aliveNum && aliveNum < pool->mThreadNumMax) {
            for (size_t i = 0, count = 0; i < pool->mThreadNumMax && THREAD_NUM_ONCE > count; ++i) {
                if (pool->mWorkThread[i].ThreadStatus() == ThreadBase::THREAD_EXIT)  {
                    pool->mWorkThread[i].setWorkFunc(worker);
                    pool->mWorkThread[i].setArg(arg);
                    pool->mWorkThread[i].run();
                    ++count;
                    LOGI("Thread %s %d is running", pool->mWorkThread[i].GetThreadName(),
                        pool->mWorkThread[i].getKernalTid());
                    {
                        AutoLock<Mutex> lock(pool->mPoolMutex);
                        pool->mAliveThreadNum++;
                    }
                }
                if (pool->mWorkThread[i].ThreadStatus() == ThreadBase::THREAD_WAITING)  {
                    pool->mWorkThread[i].setWorkFunc(worker);
                    pool->mWorkThread[i].setArg(arg);
                    pool->mWorkThread[i].StartWork();
                    LOGI("Thread %s %d is running", pool->mWorkThread[i].GetThreadName(),
                        pool->mWorkThread[i].getKernalTid());
                    ++count;
                    {
                        AutoLock<Mutex> lock(pool->mPoolMutex);
                        pool->mAliveThreadNum++;
                    }
                }
            }
        }

        // 任务减少，销毁线程
        if (queueSize < aliveNum * 2 && aliveNum > pool->mThreadNumMin) {
            pool->mPoolMutex.lock();
            pool->mExitNum = 2;
            pool->mPoolMutex.unlock();
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
    ThreadPool *pool = static_cast<ThreadPool*>(arg);
    LOG_ASSERT(pool != nullptr, "pool is nullptr");
    while (!pool->mShouldExit.load(std::memory_order_acquire)) // mShouldExit = true 退出
    {
        while (pool->mTask->taskNumber() == 0) {
            // 任务队列条件变量
            // 作用：当前任务数量为0，则需阻塞线程，使
            pool->mTask->mQueueMutex.lock();
            pool->mQueueCond.wait(pool->mTask->mQueueMutex);
            pool->mTask->mQueueMutex.unlock();
            AutoLock<Mutex> lock(pool->mPoolMutex);
            if (pool->mExitNum > 0) {
                pool->threadExit();
                return OK;
            }
        }
        Task t = pool->mTask->task();
        
        if (t.func == nullptr) {
            LOGW("task.func is nullptr");
            continue;
        }
        {
            AutoLock<Mutex> lock(pool->mPoolMutex);
            pool->mBusyNum++;
        }
        t.func(t.data);
        if (t.data) {
            delete t.data;
            t.data = nullptr;
        }
        {
            AutoLock<Mutex> lock(pool->mPoolMutex);
            pool->mBusyNum--;
        }
    }
    return OK;
}

void ThreadPool::threadExit()
{
    LOGI("ThreadPool::threadExit() Thread %d exit\n", gettid());
    pthread_exit(NULL);
}

} // namespace Alias
