/*************************************************************************
    > File Name: types.h
    > Author: hsz
    > Brief:
    > Created Time: Wed 07 Sep 2022 09:14:53 AM CST
 ************************************************************************/

#ifndef __EULAR_TYPES_H__
#define __EULAR_TYPES_H__

#include "sysdef.h"
#include <arpa/inet.h> // for htons htonl ntohs ntohl
#include <byteswap.h>
#include <type_traits>  // for enable_if

#if defined(OS_UNIX)
#define FORCEINLINE inline __attribute__((always_inline))
#endif

namespace eular {

// 8字节类型转换
template<typename T>
typename std::enable_if<sizeof(T) == sizeof(uint64_t), T>::type
byteswap(T value)
{
    return (T)bswap_64((uint64_t)value);
}

// 4字节类型转换
template<typename T>
typename std::enable_if<sizeof(T) == sizeof(uint32_t), T>::type
byteswap(T value)
{
    return (T)bswap_32((uint32_t)value);
}

// 2字节类型转换
template<typename T>
typename std::enable_if<sizeof(T) == sizeof(uint16_t), T>::type
byteswap(T value)
{
    return (T)bswap_16((uint16_t)value);
}

#if BYTE_ORDER == BIG_ENDIAN
template<typename T>
T toBigEndian(T value)
{
    return value;
}

template<typename T>
T toLittleEndian(T value)
{
    return byteswap(value);
}

#else

// 将value转换为大端字节数，在小端机执行byteswap
template<typename T>
T toBigEndian(T value)
{
    return byteswap(value);
}

// 将value转换为小端字节数，在小端机直接返回
template<typename T>
T toLittleEndian(T value)
{
    return value;
}
#endif


template<typename T>
struct little_endian
{
    little_endian() { mData = 0; }
    little_endian(const T &t) { mData = toLittleEndian(t); }
    little_endian(const little_endian &other) { mData = other.mData; }
    little_endian &operator=(const little_endian &other) {
        if (&other != this) {
            mData = other.mData;
        }
        return *this;
    }

    T operator=(const T &t) { mData = toLittleEndian(t); return t; }

    operator T() const {
    #if BYTE_ORDER == LITTLE_ENDIAN
        return mData;
    #else
        return toBigEndian(mData);
    #endif
    }

private:
    T mData;
};

typedef little_endian<int16_t>  int16s;
typedef little_endian<int32_t>  int32s;
typedef little_endian<uint16_t> uint16s;
typedef little_endian<uint32_t> uint32s;
typedef little_endian<int64_t>  int64s;
typedef little_endian<uint64_t> uint64s;

// static_assert(sizeof(int16s)  == 2);
// static_assert(sizeof(int32s)  == 4);
// static_assert(sizeof(uint16s) == 2);
// static_assert(sizeof(uint32s) == 4);
// static_assert(sizeof(int64s)  == 8);
// static_assert(sizeof(uint64s) == 8);

} // namespace eular

#endif // __EULAR_TYPES_H__