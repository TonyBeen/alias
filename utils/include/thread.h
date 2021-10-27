/*************************************************************************
    > File Name: thread.h
    > Author: hsz
    > Desc: 
    > Created Time: 2021年05月30日 星期日 10时06分34秒
 ************************************************************************/

#ifndef __THREAD_H__
#define __THREAD_H__

#include "utils.h"
#include "threaddef.h"
#include "mutex.h"
#include "condition.h"
#include "string8.h"
#include <stdint.h>
#include <atomic>

#define THREAD_FUNC_RETURN 0xFFF

namespace Jarvis {
class ThreadBase
{
public:
    typedef enum {
        THREAD_EXIT = 0,
        THREAD_RUNNING = 1,
        THREAD_WAITING = 2
    } thread_status_t;

    typedef void *(*ThreadFunc)(void *);
    ThreadBase(const char *threadName, uint8_t isThreadDetach = true);
    virtual ~ThreadBase();
    DISALLOW_COPY_AND_ASSIGN(ThreadBase);

    uint32_t        ThreadStatus() const;
    void            Interrupt();
    bool            ForceExit();
    int             run(size_t stackSize = 0);
    void            StartWork();
    const String8&  GetThreadName() const { return mThreadName; }
    const uint32_t& getKernalTid() const { return mKernalTid; }
    const pthread_t getTid() const { return mTid; }
protected:
            void    *userData;       // 用户传参数据块
    virtual int     threadWorkFunction(void *arg) = 0;
            String8 mThreadName;

private:
    static  int     threadloop(void *user);
            bool    ShouldExit();
private:
    mutable     Mutex               mMutex;
                uint32_t            mPid;
                uint32_t            mKernalTid;
                pthread_t           mTid;

    std::atomic<uint32_t>           mThreadStatusAtomic;
    volatile    bool                mExitStatus;
                bool                mIsThreadDetached;
                Condition           mCond;
};

class Thread final : public ThreadBase
{
public:
    Thread();
    Thread(const char *threadName, user_thread_function func = nullptr, uint8_t isDetach = true);
    ~Thread();

    void setThreadName(const String8& name) { mThreadName = name; }
    void setArg(void *arg);
    void setWorkFunc(user_thread_function func);
    int  getFunctionReturn();
protected:
    virtual int threadWorkFunction(void *arg);

private:
    user_thread_function function;
    volatile int  mFuncReturn;
};

} // namespace Jarvis

#endif // __THREAD_H__