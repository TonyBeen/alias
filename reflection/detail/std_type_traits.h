/*************************************************************************
    > File Name: std_type_traits.h
    > Author: hsz
    > Brief:
    > Created Time: 2024年03月16日 星期六 19时49分02秒
 ************************************************************************/

#ifndef __REFLECTION_DETAIL_STD_TYPE_TRAITS_H__
#define __REFLECTION_DETAIL_STD_TYPE_TRAITS_H__

#include <type_traits>

namespace eular {
namespace detail {

template<bool B, class T, class F>
using conditional_t = typename std::conditional<B, T, F>::type;

template<typename T>
using remove_cv_t = typename std::remove_cv<T>::type;

template<typename T>
using remove_volatile_t = typename std::remove_volatile<T>::type;

template<typename T>
using remove_const_t = typename std::remove_const<T>::type;

template<typename T>
using remove_pointer_t = typename std::remove_pointer<T>::type;

template<typename T>
using remove_reference_t = typename std::remove_reference<T>::type;

template<typename T>
using add_pointer_t = typename std::add_pointer<T>::type;

template< bool B, class T = void >
using enable_if_t = typename std::enable_if<B, T>::type;

template<typename T>
using decay_t = typename std::decay<T>::type;

template<typename T>
using add_const_t = typename std::add_const<T>::type;

template<typename T>
using add_lvalue_reference_t = typename std::add_lvalue_reference<T>::type;

///////////////////////////////////////////////////////////////////////////////

template <typename T, typename...Ts>
struct max_sizeof_list_impl;

template <typename T>
struct max_sizeof_list_impl<T>
{
    static const std::size_t value = sizeof(T);
};

template<typename T1, typename T2, typename... U>
struct max_sizeof_list_impl<T1, T2, U...>
{
    static const std::size_t value = max_sizeof_list_impl< conditional_t< sizeof(T1) >= sizeof(T2), T1, T2>, U...>::value;
};

template<template<class...> class List, class... Ts>
struct max_sizeof_list_impl<List<Ts...>>
{
    static const std::size_t value = max_sizeof_list_impl<Ts...>::value;
};

// Returns the maximum sizeof value from all given types
// use it like this:
// max_sizeof_list<int, bool, double>::value == 8
template<typename... Ts>
using max_sizeof_list = std::integral_constant<std::size_t, max_sizeof_list_impl<Ts...>::value>;


template <typename T, typename...Ts>
struct max_alignof_list_impl;

template <typename T>
struct max_alignof_list_impl<T>
{
    static constexpr size_t value = std::alignment_of<T>::value;
};

template<typename T1, typename T2, typename... U>
struct max_alignof_list_impl<T1, T2, U...>
{
    static constexpr size_t value = max_alignof_list_impl<conditional_t<std::alignment_of<T1>::value >= std::alignment_of<T2>::value, T1, T2>, U...>::value;
};

template<template<class...> class List, typename... Ts>
struct max_alignof_list_impl<List<Ts...>>
{
    static constexpr size_t value = max_alignof_list_impl<Ts...>::value;
};

// Returns the maximum sizeof value from all given types
// use it like this:
// max_alignof_list<int, bool, double>::value == 8
template<typename... Ts>
using max_alignof_list = std::integral_constant<std::size_t, max_alignof_list_impl<Ts...>::value>;

} // namespace detail
} // namespace eular

#endif // __REFLECTION_DETAIL_STD_TYPE_TRAITS_H__
