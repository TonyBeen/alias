/*************************************************************************
    > File Name: mutex.cpp
    > Author: hsz
    > Desc: 
    > Created Time: 2021年04月25日 星期日 21时25分03秒
 ************************************************************************/

#include "mutex.h"
#include <errno.h>
#include <assert.h>

namespace Jarvis {
Mutex::Mutex(int type)
{
    int ret = 0;
    if (type == SHARED) {
        pthread_mutexattr_t attr;
        pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&mMutex, &attr);
        pthread_mutexattr_destroy(&attr);
    } else {
        do {
            ret = pthread_mutex_init(&mMutex, nullptr);
        } while (ret == EAGAIN);
        assert(ret == 0);
    }
}

Mutex::~Mutex()
{
    int ret = 0;
    do {
        ret = pthread_mutex_destroy(&mMutex);;
    } while (ret == EAGAIN);
    assert(ret == 0);
}

int Mutex::lock() const
{
    return pthread_mutex_lock(&mMutex);
}

void Mutex::unlock() const
{
    pthread_mutex_unlock(&mMutex);
}

int Mutex::trylock() const
{
    return pthread_mutex_trylock(&mMutex);
}

RWMutex::RWMutex()
{
    pthread_rwlock_init(&mRWMutex, nullptr);
}

RWMutex::~RWMutex()
{
    pthread_rwlock_destroy(&mRWMutex);
}

void RWMutex::rlock()
{
    pthread_rwlock_rdlock(&mRWMutex);
}

void RWMutex::wlock()
{
    pthread_rwlock_wrlock(&mRWMutex);
}

void RWMutex::unlock()
{
    pthread_rwlock_unlock(&mRWMutex);
}

}