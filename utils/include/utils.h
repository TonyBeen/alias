/*************************************************************************
    > File Name: utilsfunc.cpp
    > Author: hsz
    > Mail:
    > Created Time: Wed May  5 14:55:59 2021
 ************************************************************************/

#ifndef __UTILS_FUNCTION_H__
#define __UTILS_FUNCTION_H__

#include <stdint.h>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/syscall.h>

#ifndef gettid
#define gettid() syscall(__NR_gettid)
#endif

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(ClassName) \
    ClassName(const ClassName&) = delete; \
    ClassName& operator=(const ClassName&) = delete;
#endif


typedef unsigned long long nsec_t;
        nsec_t  seconds(uint16_t sec);
        nsec_t  mseconds(uint16_t ms);

static  bool    __lstat(const char *path);
static  bool    __mkdir(const char *path);
        bool    Mkdir(const std::string &path);
        int     msleep(uint32_t ms);


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
