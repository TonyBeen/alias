/*************************************************************************
    > File Name: debug.h
    > Author: hsz
    > Mail:
    > Created Time: Thu 29 Jul 2021 11:02:40 PM PDT
 ************************************************************************/

#include <stdio.h>

//#define DEBUG
#ifdef DEBUG
    #ifndef SLOGD
        #define SLOGD printf
    #endif
#else
    #ifndef SLOGD
        #define SLOGD(...)
    #endif
#endif
