/*************************************************************************
    > File Name: Buffer.h
    > Author: hsz
    > Mail:
    > Created Time: Mon Jul  5 13:08:56 2021
 ************************************************************************/

#ifndef __ALIAS_BUFFER_H__
#define __ALIAS_BUFFER_H__
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <string>

namespace eular {
class ByteBuffer final
{
public:
    ByteBuffer();                       // call ByteBuffer(4096);
    ByteBuffer(size_t size);            // size == 0 ? 创建4096大小, 内容为空的buf : size
    ByteBuffer(const char *data, size_t dataLength);  // 如果data为null，则和上面一致
    ByteBuffer(const uint8_t *data, size_t dataLength);
    ByteBuffer(ByteBuffer&& other);
    ~ByteBuffer();

    ByteBuffer(const ByteBuffer& other);
    ByteBuffer& operator=(const ByteBuffer& other);
    ByteBuffer& operator=(ByteBuffer& other);
    ByteBuffer& operator=(ByteBuffer&& other);
    uint8_t&    operator[](size_t index);

    // 在offset之后设为data
    size_t      set(const uint8_t *data, size_t dataSize, size_t offset = 0);
    void        append(const uint8_t *data, size_t dataSize);
    // 在offset之后插入数据而不覆盖之后的数据
    size_t      insert(const uint8_t *data, size_t dataSize, size_t offset = 0);

    uint8_t *   data() { return mBuffer ? mBuffer : nullptr; }
    const uint8_t *const_data() const { return mBuffer ? mBuffer : nullptr; }
    const uint8_t *begin() const { return mBuffer ? mBuffer : nullptr; }                       // 返回数据开始地址
    const uint8_t *end() const { return mBuffer ? (mBuffer + mDataSize - 1) : nullptr; }       // 返回数据结束地址
    void        resize(size_t newSize);
    size_t      capacity() const { return mCapacity; }
    size_t      size() const { return mDataSize; }
    void        clear();
    void        setDataSize(size_t sz) { mDataSize = sz; }

    std::string dump()  const;

private:
    size_t      calculate(size_t);
    size_t      getBuffer(size_t size);
    void        freeBuffer();

private:
    uint8_t*    mBuffer;
    size_t      mDataSize;
    size_t      mCapacity;
};

} // namespace eular

#endif // __ALIAS_BUFFER_H__
