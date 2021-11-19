/*************************************************************************
    > File Name: singleton.h
    > Author: hsz
    > Mail:
    > Created Time: Wed 22 Sep 2021 09:34:09 AM CST
 ************************************************************************/

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <bits/move.h>

namespace Jarvis {

template<typename T>
class Singleton {
public:
    template<typename... Args>
    static T *get(Args... args)
    {
        if (mInstance == nullptr) {
            mInstance = new T(std::forward<Args>(args)...);
        }
        return mInstance;
    }

    template<typename... Args>
    static T *reset(Args... args)
    {
        free();
        mInstance = new T(std::forward<Args>(args)...);
        return mInstance;
    }

    static void free()
    {
        if (mInstance != nullptr) {
            delete mInstance;
            mInstance = nullptr;
        }
    }
private:
    static T *mInstance;

    Singleton() {}
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

template<typename T>
T *Singleton<T>::mInstance = nullptr;

} // namespace Jarvis

#endif  // __SINGLETON_H__
