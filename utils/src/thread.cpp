/*************************************************************************
    > File Name: thread.cpp
    > Author: hsz
    > Desc: 
    > Created Time: 2021年05月30日 星期日 10时06分39秒
 ************************************************************************/

#define _DEBUG

#include "thread.h"
#include "Errors.h"
#include "debug.h"
#include "exception.h"
#include <string.h>

namespace eular {
ThreadBase::ThreadBase(const char *threadName, uint8_t isThreadDetach) :
    mExitStatus(false),
    mPid(0),
    mTid(0),
    mKernalTid(0)
{
    mThreadStatusAtomic = THREAD_EXIT;
    if (threadName) {
        mThreadName = threadName;
    }
    mIsThreadDetached = isThreadDetach;
}

ThreadBase::~ThreadBase()
{
    if (ThreadStatus() != THREAD_EXIT) { // 等待线程退出，否则在析构完成之后会导致线程段错误问题
        Interrupt();
    }
}

uint32_t ThreadBase::ThreadStatus() const
{
    return mThreadStatusAtomic.load();
}

void ThreadBase::Interrupt()
{
    {
        AutoLock<Mutex> _l(mMutex);
        mExitStatus = true;
    }
    if (mThreadStatusAtomic.load() == THREAD_WAITING) { // 如果线程处于等待用户状态，则需要通知线程
        mCond.broadcast();
    }
}

bool ThreadBase::ForceExit()
{
    return pthread_cancel(mTid) == 0;
}

bool ThreadBase::ShouldExit()
{
    AutoLock<Mutex> _l(mMutex);
    // LOG("thread %d exit mutex owner %d\n", mKernalTid, mMutex.mutex()->__data.__owner);
    return mExitStatus;
}

int ThreadBase::run(size_t stackSize)
{
    AutoLock<Mutex> _lock(mMutex);
    if (mThreadStatusAtomic.load() != THREAD_EXIT) {
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
        mThreadStatusAtomic = THREAD_RUNNING;
    } else {
        LOG("pthread_create error %s\n", strerror(ret));
    }
    pthread_attr_destroy(&attr);
    return ret;
}


/**
 * @brief 如果线程运行中，直接返回；如果等待则broadcast；如果退出，则另起线程
 */
void ThreadBase::StartWork()
{
    switch (mThreadStatusAtomic.load())
    {
    case THREAD_WAITING:
        mCond.broadcast();
        break;
    case THREAD_RUNNING:
        break;
    case THREAD_EXIT:
        run();
    default:
        break;
    }
}

/**
 * @brief 线程循环，子类执行函数入口
 */
int ThreadBase::threadloop(void *user)
{
    LOG("%s() start \n", __func__);
    ThreadBase *threadBase = (ThreadBase *)user;
    threadBase->mKernalTid = gettid();
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, nullptr);    // 设置任何时间点都可以取消线程

    while (threadBase->ShouldExit() == false) {
        LOG("thread is going to run...\n");
        int result = threadBase->threadWorkFunction(threadBase->userData);
        if (result == THREAD_EXIT || threadBase->ShouldExit() == true) {
            threadBase->mThreadStatusAtomic = THREAD_EXIT;
            break;
        }

        threadBase->mThreadStatusAtomic = THREAD_WAITING;

        LOG("thread wait...\n");
        AutoLock<Mutex> _l(threadBase->mMutex);
        threadBase->mCond.wait(threadBase->mMutex);     // 阻塞线程，由用户决定下一次执行任务的时间
        threadBase->mThreadStatusAtomic = THREAD_RUNNING;
    }
    LOG("thread exit...\n");
    return 0;
}

Thread::Thread() : ThreadBase("ThreadBase")
{
    userData = nullptr;
    function = nullptr;
    mFuncReturn = THREAD_FUNC_RETURN;
}

Thread::Thread(const char *threadName, user_thread_function func, uint8_t isDetach) :
    ThreadBase(threadName, isDetach)
{
    userData = nullptr;
    this->function = func;
    mFuncReturn = THREAD_FUNC_RETURN;
}

Thread::~Thread()
{

}

int Thread::threadWorkFunction(void *arg)
{
    if (function) {
        mFuncReturn = function(arg);
        LOG("user functions return %d\n", mFuncReturn);
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

} // namespace eular
