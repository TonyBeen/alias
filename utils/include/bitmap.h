/*************************************************************************
    > File Name: bitmap.h
    > Author: hsz
    > Brief:
    > Created Time: 2022-05-23 09:42:30 Monday
 ************************************************************************/

#ifndef __EULAR_BITMAP_H__
#define __EULAR_BITMAP_H__

#include <stdint.h>
#include <stdio.h>

namespace eular {

class BitMap final
{
public:
    BitMap();
    BitMap(size_t size);
    ~BitMap();

    bool set(size_t idx, bool v);
    bool at(size_t idx) const;
    void clear();
    size_t capacity() const;
    bool resize(size_t size);

private:
    uint8_t *alloc(size_t size);
    void release();

private:
    uint8_t*    mBitMap;
    size_t      mCapacity;
};

} // namespace eular

#endif // __EULAR_BITMAP_H__
