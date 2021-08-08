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


#endif // __UTILS_FUNCTION_H__
