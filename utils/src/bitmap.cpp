/*************************************************************************
    > File Name: bitmap.cpp
    > Author: hsz
    > Brief:
    > Created Time: 2022-05-23 09:42:46 Monday
 ************************************************************************/

#include "bitmap.h"
#include <stdlib.h>
#include <string.h>

#define BIT_LENGTH_PEER_BYTE (8)
#define DEFAULT_SIZE (1024)

namespace eular {
BitMap::BitMap()
{
    mBitMap = alloc(DEFAULT_SIZE);
}

BitMap::BitMap(size_t size)
{
    mBitMap = alloc(size);
}

BitMap::~BitMap()
{
    release();
}

bool BitMap::set(size_t idx, bool v)
{
    if (idx >= mCapacity) {
        return false;
    }

    size_t index = idx / 8;
    size_t offset = idx % 8;
    if (offset) {
        ++index;
    }

    uint8_t& value = mBitMap[index];
    uint8_t temp = 0;
    if (v) {
        temp = 1 << offset;
        value |= temp;
    }
    else {
        temp = ~temp;
        temp ^= 1 << offset;
        value &= temp;
    }

    return at(idx);
}

bool BitMap::at(size_t idx) const
{
    if (idx >= mCapacity) {
        return false;
    }

    size_t index = idx / 8;
    size_t offset = idx % 8;
    if (offset) {
        ++index;
    }

    uint8_t value = mBitMap[index];
    uint8_t temp = 0x01;

    value >>= offset;
    return value & temp;
}

void BitMap::clear()
{
    memset(mBitMap, 0, mCapacity / 8);
}

size_t BitMap::capacity() const
{
    return mCapacity;
}

bool BitMap::resize(size_t size)
{
    if (size == mCapacity) {
        return true;
    }

    size_t cap = mCapacity;
    uint8_t* temp = alloc(size);
    if (temp == nullptr) {
        mCapacity = cap;
        return false;
    }

    size_t bytes = (cap > mCapacity) ? (mCapacity / 8) : (cap / 8);
    memcpy(temp, mBitMap, bytes);

    release();
    mBitMap = temp;
    return true;
}

/**
 * @brief 申请一块内存
 *
 * @param size bitmap容量
 * @return 失败为nullptr
 */
uint8_t* BitMap::alloc(size_t size)
{
    size_t bytes = (size + 8) / 8;
    mCapacity = 0;
    uint8_t* bitMap = (uint8_t*)malloc(bytes);
    if (bitMap != nullptr) {
        memset(bitMap, 0, bytes);
        mCapacity = bytes * 8;
    }

    return bitMap;
}

void BitMap::release()
{
    if (mBitMap) {
        free(mBitMap);
        mBitMap = nullptr;
    }
}

} // namespace eular
