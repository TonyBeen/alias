/*************************************************************************
    > File Name: thread.cpp
    > Author: hsz
    > Desc: 
    > Created Time: 2021年05月30日 星期日 10时06分39秒
 ************************************************************************/

#include "thread.h"
#include "Errors.h"
#include <string.h>

#define DEBUG
#ifdef DEBUG
    #define SLOGD printf
#else
    #define SLOGD(...)
#endif

namespace Alias {
ThreadBase::ThreadBase(const char *threadName, uint8_t isThreadDetach) :
    mThreadStatus(THREAD_EXIT),
    mExitStatus(false),
    mPid(0),
    mTid(0),
    mKernalTid(0)
{
    if (threadName) {
        mThreadName = threadName;
    }
    mIsThreadDetached = isThreadDetach;
}

ThreadBase::~ThreadBase()
{

}

ThreadBase::thread_status_t ThreadBase::ThreadStatus() const
{
    AutoLock<Mutex> _l(mMutex);
    // __data.__owner -> 表示当前持锁线程
    // SLOGD("thread %d status mutex owner %d\n", mKernalTid, mStatusMutex.mutex()->__data.__owner);
    return mThreadStatus;
}

void ThreadBase::Interrupt()
{
    {
        AutoLock<Mutex> _l(mMutex);
        mExitStatus = true;
    }

    mCond.broadcast();  // 如果线程处于等待用户状态，则需要通知线程
}

bool ThreadBase::ShouldExit()
{
    AutoLock<Mutex> _l(mMutex);
    // SLOGD("thread %d exit mutex owner %d\n", mKernalTid, mMutex.mutex()->__data.__owner);
    return mExitStatus;
}

int ThreadBase::run(size_t stackSize)
{
    AutoLock<Mutex> _lock(mMutex);
    if (mThreadStatus != THREAD_EXIT) {
        return INVALID_OPERATION;
    }
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    if (stackSize) {
        pthread_attr_setstacksize(&attr, stackSize);
    }
    if (mIsThreadDetached) {
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    }

    int ret = pthread_create(&mTid, &attr, (ThreadFunc)threadloop, this);
    if (ret == 0) {
        mThreadStatus = THREAD_RUNNING;
    } else {
        SLOGD("pthread_create error %s\n", strerror(ret));
        return ret;
    }
    pthread_attr_destroy(&attr);
    return 0;
}

void ThreadBase::StartWork()
{
    mCond.broadcast();
}

int ThreadBase::threadloop(void *user)
{
    SLOGD("%s() start \n", __func__);
    ThreadBase *threadBase = (ThreadBase *)user;
    threadBase->mKernalTid = gettid();
    while (threadBase->ShouldExit() == false) {
        SLOGD("thread is going to run...\n");
        int result = threadBase->threadWorkFunction(threadBase->userData);

        if (result == THREAD_EXIT || threadBase->ShouldExit() == true) {
            AutoLock<Mutex> _l(threadBase->mMutex);
            threadBase->mThreadStatus = THREAD_EXIT;
            break;
        }

        {
            AutoLock<Mutex> _l(threadBase->mMutex);
            threadBase->mThreadStatus = THREAD_WAITING;
        }
        SLOGD("thread wait...\n");
        threadBase->mCond.wait(threadBase->mMutex);     // 阻塞线程，由用户决定下一次执行任务的时间
        threadBase->mThreadStatus = THREAD_RUNNING;
        threadBase->mMutex.unlock();
    }
    SLOGD("Thread exit...\n");
    return 0;
}

Thread::Thread() : ThreadBase("ThreadBase")
{
    userData = nullptr;
    function = nullptr;
    mFuncReturn = 0xFFFF;
}

Thread::Thread(const char *threadName, user_thread_function func, uint8_t isDetach) :
    ThreadBase(threadName, isDetach)
{
    userData = nullptr;
    this->function = func;
    mFuncReturn = 0xFFFF;
}

Thread::~Thread()
{
    while (ThreadStatus() != THREAD_EXIT) // 等待线程退出，否则在析构完成之后会导致线程段错误问题
    {
        Interrupt();
        msleep(1);
    }
}

int Thread::threadWorkFunction(void *arg)
{
    if (function) {
        mFuncReturn = function(arg);
        SLOGD("user functions return %d\n", mFuncReturn);
    }
    return mFuncReturn;
}

int Thread::getFunctionReturn()
{
    return mFuncReturn;
}

void Thread::setArg(void *arg)
{
    this->userData = arg;
}

void Thread::setWorkFunc(user_thread_function func)
{
    this->function = func;
}

} // namespace Alias
