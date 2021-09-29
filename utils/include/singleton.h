/*************************************************************************
    > File Name: singleton.h
    > Author: hsz
    > Mail:
    > Created Time: Wed 22 Sep 2021 09:34:09 AM CST
 ************************************************************************/

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <memory>
using std::shared_ptr;

namespace Jarvis {

/**
 * @brief 单例, 无参构造
 */
template<typename DataType>
class Singleton
{
public:
    static DataType *GetInstacnce()
    {
        static DataType t;
        return &t;
    }
    ~Singleton() {}
private:
    Singleton() {}
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

/**
 * @brief 智能指针单例
 */
template<typename DataType>
class SingletonPtr
{
public:
    typedef std::shared_ptr<DataType> ptr;
    SingletonPtr() {}
    ~SingletonPtr() {}
};

} // namespace Jarvis

#endif  // __SINGLETON_H__
