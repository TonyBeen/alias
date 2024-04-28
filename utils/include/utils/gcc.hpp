/*************************************************************************
    > File Name: gcc.hpp
    > Author: hsz
    > Brief:
    > Created Time: Fri 28 Apr 2023 11:13:57 AM CST
 ************************************************************************/

#ifndef __GCC_HPP__
#define __GCC_HPP__

#define BOOST_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#if !defined(__CUDACC__) // nvcc预定义宏, 编译CUDA源文件时预定义
#define BOOST_GCC BOOST_GCC_VERSION
#endif

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus >= 201103L)
#  define BOOST_GCC_CXX11
#endif

// GCC prior to 3.4 had #pragma once too but it didn't work well with filesystem links
#if BOOST_GCC_VERSION >= 30400
#define BOOST_HAS_PRAGMA_ONCE
#endif

// RTTI and typeinfo detection is possible post gcc-4.3:
#if BOOST_GCC_VERSION > 40300
#  ifndef __GXX_RTTI
#     ifndef BOOST_NO_TYPEID
#        define BOOST_NO_TYPEID
#     endif
#     ifndef BOOST_NO_RTTI
#        define BOOST_NO_RTTI
#     endif
#  endif
#endif

// C++0x features in 4.3.n and later
//
#if (BOOST_GCC_VERSION >= 40300) && defined(BOOST_GCC_CXX11)
// C++0x features are only enabled when -std=c++0x or -std=gnu++0x are
// passed on the command line, which in turn defines
// __GXX_EXPERIMENTAL_CXX0X__.
#  define BOOST_HAS_DECLTYPE
#  define BOOST_HAS_RVALUE_REFS
#  define BOOST_HAS_STATIC_ASSERT
#  define BOOST_HAS_VARIADIC_TMPL
#else
#  define BOOST_NO_CXX11_DECLTYPE
#  define BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS
#  define BOOST_NO_CXX11_RVALUE_REFERENCES
#  define BOOST_NO_CXX11_STATIC_ASSERT
#endif

#endif