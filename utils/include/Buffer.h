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

#define DEFAULT_BUFFER_SIZE (4096)


namespace Jarvis {
class ByteBuffer final
{
public:
    ByteBuffer();                       // call ByteBuffer(4096);
    ByteBuffer(size_t size);            // size == 0 ? 创建4096大小, 内容为空的buf : size
    ByteBuffer(const char *data, size_t dataLength);  // 如果data为null，则和上面一致
    ByteBuffer(ByteBuffer&& other);
    ~ByteBuffer();

    ByteBuffer(const ByteBuffer& other);
    ByteBuffer& operator=(const ByteBuffer& other);
    ByteBuffer& operator=(ByteBuffer& other);
    ByteBuffer& operator=(ByteBuffer&& other);
    char&       operator[](size_t index);

    // 在offset之后设为data
    size_t      set(const char *data, size_t dataSize, size_t offset = 0);
    void        append(const char *data, size_t dataSize);
    // 在offset之后插入数据而不覆盖之后的数据
    size_t      insert(const char *data, size_t dataSize, size_t offset = 0);

    char*       data() { return mBuffer ? mBuffer : nullptr; }
    const char *const_data() const { return mBuffer ? mBuffer : nullptr; }
    const char* begin() const { return mBuffer ? mBuffer : nullptr; }                       // 返回数据开始地址
    const char* end() const { return mBuffer ? (mBuffer + mDataSize - 1) : nullptr; }       // 返回数据结束地址
    void        resize(size_t newSize);
    size_t      capacity() const { return mCapacity; }
    size_t      size() const { return mDataSize; }
    void        clear();

private:
    size_t      calculate(size_t);
    size_t      getBuffer(size_t size = DEFAULT_BUFFER_SIZE);
    void        freeBuffer();

private:
    char*   mBuffer;
    size_t  mDataSize;
    size_t  mCapacity;
};

} // namespace Jarvis

#endif // __ALIAS_BUFFER_H__
