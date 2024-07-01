/*************************************************************************
    > File Name: singleton.h
    > Author: hsz
    > Mail:
    > Created Time: Wed 22 Sep 2021 09:34:09 AM CST
 ************************************************************************/

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include "mutex.h"
#include "singleton_object.h"
#include <bits/move.h>

namespace eular {

template<typename T>
class Singleton {
public:
    template<typename... Args>
    static SObject<T> Get(Args&&... args)
    {
        // 编译期间检测类型完整性
        static_assert(sizeof(T), "incomplete type");
        AutoLock<Mutex> lock(mMutex);
        if (mInstance == nullptr) {
            mInstance = new T(std::forward<Args>(args)...);
            mDeleter.registration(); // 模板静态成员变量需要使用才会构造
            // ::atexit(Singleton<T>::free); // 在mian结束后调用free函数
        }
        SObject<T> obj(mInstance, &mRef);
        return obj;
    }

    /**
     * @brief 重置实例, 会返回一个新的地址，所以原来的会失效，对于单例模式，此方法用的不太多 
     */
    template<typename... Args>
    static SObject<T> Reset(Args&&... args)
    {
        AutoLock<Mutex> lock(mMutex);
        if (mRef.load() == 0) {
            if (mInstance != nullptr) {
                delete mInstance;
                mInstance = nullptr;
            }
            mInstance = new T(std::forward<Args>(args)...);
        }

        SObject<T> obj(mInstance, &mRef);
        return obj;
    }

    static void Free()
    {
        if (mRef.load() > 0) {
            return;
        }

        AutoLock<Mutex> lock(mMutex);
        if (mInstance != nullptr) {
            delete mInstance;
            mInstance = nullptr;
        }
    }

private:
    class Deleter {
    public:
        void registration() { }
        ~Deleter()
        {
            Singleton<T>::Free();
        }
    };

private:
    static T *mInstance;
    static RefCount mRef;
    static Mutex mMutex;
    static Deleter mDeleter;

    Singleton() {}
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

template<typename T>
T *Singleton<T>::mInstance = nullptr;

template<typename T>
Mutex Singleton<T>::mMutex;

template<typename T>
RefCount Singleton<T>::mRef;

template<typename T>
typename Singleton<T>::Deleter Singleton<T>::mDeleter;

} // namespace eular

#endif  // __SINGLETON_H__
