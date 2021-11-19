/*************************************************************************
    > File Name: threadpool.cpp
    > Author: hsz
    > Mail:
    > Created Time: Wed 14 Jul 2021 11:29:15 AM CST
 ************************************************************************/

#include "threadpool.h"
#include <utils/utils.h>
#include <utils/exception.h>
#include <utils/Errors.h>
#include <log/log.h>

#define LOG_TAG "ThreadPool"

namespace Jarvis {
static const uint32_t THREAD_NUM_ONCE = 2;

static uint64_t gExcuteCount = 0;

void TaskQueue::addTask(const Task& task, bool insertFront)
{
    AutoLock<Mutex> lock(mQueueMutex);
    if (insertFront) {
        mTaskQueue.insert(mTaskQueue.begin(), task);
    } else {
        mTaskQueue.push_back(task);
    }
}

Task TaskQueue::front()
{
    AutoLock<Mutex> lock(mQueueMutex);
    gExcuteCount++;
    Task t;
    auto it = mTaskQueue.begin();
    if (it == mTaskQueue.end()) {
        return t;
    }
    t = *it;
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
        throw Exception("Create TaskQueue error: no memory");
    }
    mManagerThread = new Thread("ThreadPool Manager thread", manager);
    if (mManagerThread == nullptr) {
        LOGE("Create manager thread error: no memory");
        throw Exception("Create manager thread error: no memory");
    }
    mManagerThread->setWorkFunc(manager);
    mManagerThread->setArg(this);

    mWorkThread = new Thread[mThreadNumMax];
    if (mWorkThread == nullptr) {
        LOGE("Create worker thread error: no memory");
        throw Exception("Create worker thread error: no memory");
    }
    for (size_t i = 0; i < mThreadNumMin; ++i) {
        mWorkThread[i].setWorkFunc(worker);
        mWorkThread[i].setArg(this);
    }
    LOGD("%s() end", __func__);
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
    LOGD("%s() end", __func__);
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
    if (mWorkThread == nullptr || mManagerThread == nullptr) {
        return;
    }
    mManagerThread->run();
    for (int i = 0; i < mThreadNumMin; ++i) {
        mWorkThread[i].run();
    }
    mAliveThreadNum.store(mThreadNumMin);
    LOGD("startWork() end");
}

int ThreadPool::manager(void *arg)
{
    ThreadPool *pool = static_cast<ThreadPool*>(arg);
    LOG_ASSERT(pool != nullptr, "pool is nullptr");
    LOGD("manager() start");
    while (!pool->mShouldExit.load(std::memory_order_acquire))
    {
        // 管理者线程每1s检查一次
        msleep(1000);

        pool->mPoolMutex.lock();
        int queueSize = pool->mTask->taskNumber();
        int busyNum = pool->mBusyNum.load();
        pool->mPoolMutex.unlock();
        int aliveNum = pool->mAliveThreadNum.load();

        LOGD("%s() queueSize = %d, busyNum = %d, aliveNum = %d, minThreadNum = %d, maxThreadNum = %d", 
            __func__, queueSize, busyNum, aliveNum, pool->mThreadNumMin, pool->mThreadNumMax);
        // 任务过多，添加线程
        if ((queueSize / 4 > aliveNum) && (aliveNum < pool->mThreadNumMax)) {
            for (int i = 0, count = 0; i < pool->mThreadNumMax && THREAD_NUM_ONCE > count; ++i) {
                if (pool->mWorkThread[i].ThreadStatus() != ThreadBase::THREAD_RUNNING) {
                    pool->mWorkThread[i].setWorkFunc(worker);
                    pool->mWorkThread[i].setArg(arg);
                    pool->mWorkThread[i].StartWork();
                    LOGI("%s() Thread %s %d is running", __func__, pool->mWorkThread[i].GetThreadName(),
                        pool->mWorkThread[i].getKernalTid());
                    {
                        AutoLock<Mutex> lock(pool->mPoolMutex);
                        pool->mAliveThreadNum++;
                    }
                    ++count;
                }
            }
        }

        // 任务减少，销毁线程
        if (queueSize < aliveNum * 2 && aliveNum > pool->mThreadNumMin) {
            pool->mExitNum.store(THREAD_NUM_ONCE);
            for (size_t i = 0; i < 2; ++i) {
                pool->mQueueCond.signal();
            }
        }
    }
    // 线程退出，回收处理
    LOGI("%s() manager thread will exit", __func__);
    for (size_t i = 0; i < pool->mThreadNumMax; ++i) {
        if (pool->mWorkThread[i].ThreadStatus() == ThreadBase::THREAD_EXIT) {
            continue;
        } else {
            pool->mWorkThread[i].Interrupt();
        }
    }
    return Thread::THREAD_EXIT;
}

int  ThreadPool::worker(void *arg)
{
    ThreadPool *pool = static_cast<ThreadPool *>(arg);
    LOG_ASSERT(pool != nullptr, "pool is nullptr");
    while (!pool->mShouldExit.load(std::memory_order_acquire)) // mShouldExit = true 退出
    {
        // 当任务数量小于线程存活数
        while (pool->mTask->taskNumber() == 0) {
            // 任务队列条件变量
            // 作用：当前任务数量为0，则需阻塞线程
            AutoLock<Mutex> lock(pool->mTask->mQueueMutex);
            LOGI("worker thread %ld wait", gettid());
            pool->mQueueCond.wait(pool->mTask->mQueueMutex);
            if (pool->mExitNum.load() > 0) {
                pool->mAliveThreadNum--;
                pool->mExitNum--;
                pool->threadExit();
                return Thread::THREAD_WAITING;
            }
        }

        const Task &t = pool->mTask->front();
        if (t.cb == nullptr) {
            LOGW("task.func is nullptr");
            continue;
        }
        LOGD("excute callback");
        pool->mBusyNum++;
        t.cb();
        pool->mBusyNum--;
        LOGD("excute callback over");
    }
    return Thread::THREAD_WAITING;
}

void ThreadPool::threadExit()
{
    LOGI("ThreadPool::threadExit() Thread %d exit\n", gettid());
    // pthread_exit(NULL);
}

} // namespace Jarvis
