/*************************************************************************
    > File Name: mutex.cpp
    > Author: hsz
    > Desc: 
    > Created Time: 2021年04月25日 星期日 21时25分03秒
 ************************************************************************/

#include "mutex.h"
#include "exception.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <string.h>

namespace Jarvis {
Mutex::Mutex(int type) :
    mName("NONE")
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
        if (ret == EBUSY) {
            unlock();
        }
        ret = pthread_mutex_destroy(&mMutex);
    } while (ret == EBUSY);
    assert(ret == 0);
}

int Mutex::lock()
{
    return pthread_mutex_lock(&mMutex);
}

void Mutex::unlock()
{
    pthread_mutex_unlock(&mMutex);
}

int Mutex::trylock()
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

void RWMutex::rlock() const
{
    pthread_rwlock_rdlock(&mRWMutex);
}

void RWMutex::wlock() const
{
    pthread_rwlock_wrlock(&mRWMutex);
}

void RWMutex::unlock() const
{
    pthread_rwlock_unlock(&mRWMutex);
}


union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific) */
};

Sem::Sem(const char *semPath, uint8_t val)
{
    if (semPath == nullptr) {
        throw Exception("the first param can not be null");
    }

    // 如果信号量已存在，则后两个参数会忽略，详见man sem_open
    mSem = sem_open(semPath, O_CREAT | O_RDWR, 0664, val);
    if (mSem == SEM_FAILED) {
        String8 erorMsg = String8::format("sem_open failed. %d %s", errno, strerror(errno));
        throw Exception(erorMsg);
    }

    isNamedSemaphore = true;
}

Sem::Sem(uint8_t valBase)
{
    mSem = new sem_t;
    if (mSem == nullptr) {
        throw Exception("new sem_t error. no more memory");
    }

    if (sem_init(mSem, false, valBase)) {
        printf("%s() sem_init error %d, %s", __func__, errno, strerror(errno));
    }
    isNamedSemaphore = false;
}

Sem::~Sem()
{
    if (isNamedSemaphore) {
        sem_close(mSem);
        sem_unlink(mFilePath.c_str());
    } else {
        sem_destroy(mSem);
        delete mSem;
    }
}

bool Sem::post()
{
    return 0 == sem_post(mSem);
}

bool Sem::wait()
{
    return 0 == sem_wait(mSem);
}

bool Sem::trywait()
{
    return 0 == sem_trywait(mSem);
}

bool Sem::timedwait(uint32_t ms)
{
    struct timespec expire;
    clock_gettime(CLOCK_MONOTONIC, &expire);
    expire.tv_sec += ms / 1000;
    expire.tv_nsec += ms % 1000;
    if (expire.tv_nsec > 1000 * 1000 * 1000) {
        ++expire.tv_sec;
        expire.tv_nsec -= 1000 * 1000 * 1000;
    }

    return 0 == sem_timedwait(mSem, &expire);
}

}