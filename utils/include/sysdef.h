/*************************************************************************
    > File Name: sysdef.h
    > Author: hsz
    > Mail:
    > Created Time: Mon 27 Sep 2021 08:53:20 AM CST
 ************************************************************************/

#ifndef __SYSDEF_H__
#define __SYSDEF_H__

#include <stdio.h>

#if defined(WIN32) || defined(WIN64)
#define OS_WINDOWS
#elif defined(__linux__)
#define OS_UNIX
#endif

#if defined(OS_WINDOWS)
#ifdef SYS_DLL
    #define DLL_PORT __declspec(dllexport)
#else
    #define DLL_PORT __declspec(dllimport)
#endif
#elif defined(OS_UNIX)
    #define DLL_PORT
#endif

#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
#define NORETURN __attribute__((__noreturn__))
#define UNUSED __attribute__((__unused__))
#else
#define NORETURN
#define UNUSED
#endif

#ifndef NULL
#define NULL (void *)0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#endif // __SYSDEF_H__
