/*************************************************************************
    > File Name: debug.h
    > Author: hsz
    > Mail:
    > Created Time: Thu 29 Jul 2021 11:02:40 PM PDT
 ************************************************************************/
#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>

#ifdef _DEBUG
    #ifndef LOG
        #define LOG printf
    #endif
#else
    #ifndef LOG
        #define LOG(...)
    #endif
#endif

#endif