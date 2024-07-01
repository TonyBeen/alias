/*************************************************************************
    > File Name: std_version.h
    > Author: hsz
    > Brief:
    > Created Time: 2024年03月16日 星期六 12时20分10秒
 ************************************************************************/

#ifndef __RELFECTION_DETAIL_STD_VERSION_H__
#define __RELFECTION_DETAIL_STD_VERSION_H__

#include <utility>

#define CPP_VERSION_98 1997
#define CPP_VERSION_11 2011
#define CPP_VERSION_14 2014
#define CPP_VERSION_17 2017
#define CPP_VERSION_20 2020

#if __cplusplus == 201103L
    #define CPP_VERSION CPP_VERSION_11
#elif __cplusplus == 201402L
    #define CPP_VERSION CPP_VERSION_14
#elif __cplusplus == 201703L
    #define CPP_VERSION CPP_VERSION_17
#elif __cplusplus == 202002L
    #define CPP_VERSION CPP_VERSION_20
#else
    #define CPP_VERSION CPP_VERSION_98
#endif

#endif // __RELFECTION_DETAIL_STD_VERSION_H__