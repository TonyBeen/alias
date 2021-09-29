/*************************************************************************
    > File Name: sysdef.h
    > Author: hsz
    > Mail:
    > Created Time: Mon 27 Sep 2021 08:53:20 AM CST
 ************************************************************************/

#ifndef __SYSDEF_H__
#define __SYSDEF_H__

#include <stdio.h>

#if defined(WIN32)
#define WINDOWS
#elif defined(__linux__)
#define OS_UNIX
#endif

#if defined(WINDOWS)
#ifdef SYS_DLL
    #define DLL_PORT __declspec(dllexport)
#else
    #define DLL_PORT __declspec(dllimport)
#endif
#elif defined(OS_UNIX)
    #define DLL_PORT
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
