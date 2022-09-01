/*************************************************************************
    > File Name: singleton.h
    > Author: hsz
    > Mail:
    > Created Time: Wed 22 Sep 2021 09:34:09 AM CST
 ************************************************************************/

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include "mutex.h"
#include "exception.h"
#include <bits/move.h>
#include <stdlib.h>
#include <atomic>

namespace eular {

template<typename T>
class Singleton;

template <typename T>
class RefCount
{
public:
    RefCount() : mRefCount(0) {}
    ~RefCount() {}

    uint32_t ref()
    {
        return ++mRefCount;
    }

    uint32_t deref()
    {
        return --mRefCount;
    }

    uint32_t refCount() const
    {
        return mRefCount;
    }

private:
    std::atomic<uint32_t> mRefCount;
    friend class Singleton<T>;
};

template<typename T>
class Singleton {
public:
    struct SObject
    {
        SObject(T *obj, RefCount<T> *ref) :
            mObj(obj),
            mRef(ref)
        {
            assert(mRef != nullptr);
            mRef->ref();
        }

        SObject(const SObject &other)
        {
            mObj = other.mObj;
            mRef = other.mRef;
            mRef->ref();
        }

        SObject &operator=(const SObject &other)
        {
            mObj = other.mObj;
            mRef = other.mRef;
            mRef->ref();
            return *this;
        }

        ~SObject()
        {
            mObj = nullptr;
            mRef->deref();
            mRef = nullptr;
        }

        T *operator->()
        {
            if (mObj == nullptr) {
                throw Exception("nullptr object");
            }
            return mObj;
        }

        T &operator*()
        {
            if (mObj == nullptr) {
                throw Exception("nullptr object");
            }
            return *mObj;
        }

        operator T*()
        {
            return mObj;
        }

        operator const T*() const
        {
            return mObj;
        }
    private:
        T *mObj;
        RefCount<T> *mRef;
    };

    template<typename... Args>
    static SObject get(Args... args)
    {
        // 编译期间检测类型完整性，不完整编译不过
        static_assert(sizeof(T), "Incomplete type T");
        AutoLock<Mutex> lock(mMutex);
        if (mInstance == nullptr) {
            mInstance = new T(std::forward<Args>(args)...);
            ::atexit(Singleton::free); // 在mian结束后调用free函数
        }
        SObject obj(mInstance, &mRef);
        return obj;
    }

    /**
     * @brief 重置实例, 会返回一个新的地址，所以原来的会失效，对于单例模式，此方法用的不太多 
     */
    template<typename... Args>
    static SObject reset(Args... args)
    {
        AutoLock<Mutex> lock(mMutex);
        if (mRef.refCount() == 0) {
            if (mInstance != nullptr) {
                delete mInstance;
                mInstance = nullptr;
            }
            mInstance = new T(std::forward<Args>(args)...);
        }

        SObject obj(mInstance, &mRef);
        return obj;
    }

    static void free()
    {
        AutoLock<Mutex> lock(mMutex);
        if (mRef.refCount() > 0) {
            return;
        }

        if (mInstance != nullptr) {
            delete mInstance;
            mInstance = nullptr;
        }
    }

private:
    static T *mInstance;
    static RefCount<T> mRef;
    static Mutex mMutex;

    Singleton() {}
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

template<typename T>
T *Singleton<T>::mInstance = nullptr;

template<typename T>
Mutex Singleton<T>::mMutex;

template<typename T>
RefCount<T> Singleton<T>::mRef;

} // namespace eular

#endif  // __SINGLETON_H__
