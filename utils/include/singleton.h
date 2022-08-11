/*************************************************************************
    > File Name: singleton.h
    > Author: hsz
    > Mail:
    > Created Time: Wed 22 Sep 2021 09:34:09 AM CST
 ************************************************************************/

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include "mutex.h"
#include <bits/move.h>
#include <stdlib.h>

namespace eular {

template<typename T>
class Singleton {
public:
    template<typename... Args>
    static T *get(Args... args)
    {
        // 编译期间检测类型完整性，不完整编译不过
        static_assert(sizeof(T), "Incomplete type T");
        AutoLock<Mutex> lock(mMutex);
        if (mInstance == nullptr) {
            mInstance = new T(std::forward<Args>(args)...);
            ::atexit(Singleton::free); // 在mian结束后调用free函数
        }
        return mInstance;
    }

    /**
     * @brief 重置实例, 会返回一个新的地址，所以原来的会失效，对于单例模式，此方法用的不太多 
     */
    template<typename... Args>
    static T *reset(Args... args)
    {
        AutoLock<Mutex> lock(mMutex);
        free();
        mInstance = new T(std::forward<Args>(args)...);
        return mInstance;
    }

    static void free()
    {
        AutoLock<Mutex> lock(mMutex);
        if (mInstance != nullptr) {
            delete mInstance;
            mInstance = nullptr;
        }
    }

private:
    static T *mInstance;
    static Mutex mMutex;

    Singleton() {}
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

template<typename T>
T *Singleton<T>::mInstance = nullptr;

template<typename T>
Mutex Singleton<T>::mMutex;

} // namespace eular

#endif  // __SINGLETON_H__
