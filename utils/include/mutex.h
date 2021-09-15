/*************************************************************************
    > File Name: mutex.h
    > Author: hsz
    > Desc: for mutex
    > Created Time: 2021年04月25日 星期日 21时24分54秒
 ************************************************************************/

#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <pthread.h>

namespace Jarvis {

class NonCopyAble {
public:
    NonCopyAble() = default;
    ~NonCopyAble() = default;
    NonCopyAble(const NonCopyAble&) = delete;
    NonCopyAble& operator=(const NonCopyAble&) = delete;
};

template<typename MutexType>
class AutoLock : public NonCopyAble{
public:
    AutoLock(const MutexType& mutex) : mMutex(mutex)
    {
        mMutex.lock();
    }
    ~AutoLock()
    {
        mMutex.unlock();
    }
private:
    const MutexType& mMutex;
};

class Condition;

class Mutex : public NonCopyAble {
public:
    enum {
        PRIVATE = 0,
        SHARED = 1
    };

    Mutex(int type = PRIVATE);
    ~Mutex();
    int  lock() const;
    void unlock() const;
    int  trylock() const;
    operator pthread_mutex_t *() { return &mMutex; }
    pthread_mutex_t *mutex() { return &mMutex; }
private:
    friend class Condition;
    mutable pthread_mutex_t mMutex;
};

} // namespace Jarvis 

#endif // __MUTEX_H__
