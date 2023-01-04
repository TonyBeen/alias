/*************************************************************************
    > File Name: mutex.h
    > Author: hsz
    > Desc: for mutex
    > Created Time: 2021年04月25日 星期日 21时24分54秒
 ************************************************************************/

#ifndef __MUTEX_H__
#define __MUTEX_H__

#include "string8.h"
#include <stdint.h>
#include <atomic>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <assert.h>

namespace eular {
class NonCopyAble
{
public:
    NonCopyAble() = default;
    ~NonCopyAble() = default;
    NonCopyAble(const NonCopyAble&) = delete;
    NonCopyAble& operator=(const NonCopyAble&) = delete;
};

template<typename MutexType>
class AutoLock : public NonCopyAble
{
public:
    AutoLock(MutexType& mutex) : mMutex(mutex)
    {
        mMutex.lock();
    }
    ~AutoLock()
    {
        mMutex.unlock();
    }
private:
    MutexType& mMutex;
};

class Mutex : public NonCopyAble
{
public:
    enum {
        PRIVATE = 0,
        SHARED = 1
    };

    void setMutexName(const String8 &name);
    const String8 &getMutexName() const { return mName; }
    Mutex(int type = PRIVATE);
    ~Mutex();
    int  lock();
    void unlock();
    int  trylock();

    operator pthread_mutex_t *() { return &mMutex; }
    pthread_mutex_t *mutex() { return &mMutex; }

private:
    friend class Condition;
    mutable pthread_mutex_t mMutex;
    String8 mName;
};

// 局部写锁
template<typename WRMutexType>
class WRAutoLock 
{
public:
    WRAutoLock(WRMutexType& mtx) : mutex(mtx)
    {
        mutex.wlock();
    }
    ~WRAutoLock()
    {
        mutex.unlock();
    }

private:
    WRMutexType &mutex;
};

// 局部读锁
template<typename RDMutexType>
class RDAutoLock
{
public:
    RDAutoLock(RDMutexType& mtx) : mutex(mtx)
    {
        mutex.rlock();
    }
    ~RDAutoLock()
    {
        mutex.unlock();
    }

private:
    RDMutexType &mutex;
};

// 读写锁，读共享，写独享，读上锁无法写，写上锁无法读写
class RWMutex : public NonCopyAble {
public:
    typedef RDAutoLock<RWMutex> ReadAutoLock;
    typedef WRAutoLock<RWMutex> WriteAutoLock;

    RWMutex();
    ~RWMutex();
    void rlock();
    void wlock();
    void unlock();

private:
    mutable pthread_rwlock_t mRWMutex;
#ifdef DEBUG
    std::atomic<bool> mReadLocked;
    std::atomic<bool> mWritLocked;
#endif
};

class Sem : public NonCopyAble {
public:
    Sem(const char *semPath, uint8_t val);      // 此种走有名信号量
    Sem(uint8_t valBase);                       // 此种走无名信号量
    ~Sem();

    bool post();
    bool wait();
    bool trywait();
    bool timedwait(uint32_t ms);

private:
    sem_t  *mSem;       // 信号量
    String8 mFilePath;  // 有名信号量使用
    bool    isNamedSemaphore;
};

} // namespace eular

#endif // __MUTEX_H__
