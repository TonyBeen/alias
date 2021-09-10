/*************************************************************************
    > File Name: utilsfunc.cpp
    > Author: hsz
    > Mail:
    > Created Time: Wed May  5 14:55:59 2021
 ************************************************************************/

#ifndef __UTILS_FUNCTION_H__
#define __UTILS_FUNCTION_H__

#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <string>
#include <vector>

#ifndef gettid
#define gettid() syscall(__NR_gettid)
#endif

#ifdef DISALLOW_COPY_AND_ASSIGN
#undef DISALLOW_COPY_AND_ASSIGN
#endif
#define DISALLOW_COPY_AND_ASSIGN(ClassName) \
    ClassName(const ClassName&) = delete; \
    ClassName& operator=(const ClassName&) = delete;

#define atomic_or(P, V) __sync_or_and_fetch((P), (V))       // p: 地址 V: 值，P指向的内容与V相或
#define atomic_and(P, V) __sync_and_and_fetch((P), (V))
#define atomic_add(P, V) __sync_add_and_fetch((P), (V))
#define atomic_xadd(P, V) __sync_fetch_and_add((P), (V))

// P: 地址 O: 旧值 N: 新值; if (O == *P) *p = N;   返回旧值
#define cmpxchg(P, O, N) __sync_val_compare_and_swap((P), (O), (N)) 

typedef unsigned long long nsec_t;
        nsec_t  seconds(uint16_t sec);
        nsec_t  mseconds(uint16_t ms);

static  bool        __lstat(const char *path);
static  bool        __mkdir(const char *path);
        bool        Mkdir(const std::string &path);
        int         msleep(uint32_t ms);

std::vector<int>    getPidByName(const char *procName);
std::string         getNameByPid(pid_t pid);

std::string Time2Str(time_t ts, const std::string& format = "%Y-%m-%d %H:%M:%S");
time_t Str2Time(const char* str, const char* format = "%Y-%m-%d %H:%M:%S");

class TypeUtil
{
public:
    static int8_t   ToChar(const std::string& str);
    static int64_t  Atoi(const std::string& str);
    static double   Atof(const std::string& str);
    static int8_t   ToChar(const char* str);
    static int64_t  Atoi(const char* str);
    static double   Atof(const char* str);
};

#endif // __UTILS_FUNCTION_H__
