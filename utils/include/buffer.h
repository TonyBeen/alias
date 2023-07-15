/*************************************************************************
    > File Name: Buffer.h
    > Author: hsz
    > Mail:
    > Created Time: Mon Jul  5 13:08:56 2021
 ************************************************************************/

#ifndef __EULAR_BUFFER_H__
#define __EULAR_BUFFER_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <string>

namespace eular {
class ByteBuffer final
{
public:
    ByteBuffer();
    ByteBuffer(size_t size);
    ByteBuffer(const char *data, size_t dataLength = SIZE_MAX);
    ByteBuffer(const uint8_t *data, size_t dataLength);
    ByteBuffer(const ByteBuffer& other);
    ByteBuffer(ByteBuffer&& other);
    ~ByteBuffer();

    ByteBuffer& operator=(const ByteBuffer& other);
    ByteBuffer& operator=(ByteBuffer&& other);
    uint8_t&    operator[](size_t index);

    // 在offset之后设为data
    size_t      set(const uint8_t *data, size_t dataSize, size_t offset = 0);
    void        append(const char *data, size_t dataSize = SIZE_MAX);
    void        append(const uint8_t *data, size_t dataSize);
    void        append(const ByteBuffer &other);
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
    void        setDataSize(size_t sz) { mDataSize = sz > mCapacity ? mCapacity : sz; }

    bool        write(const void *data, size_t size);
    bool        read(void *data, size_t size);

    bool        writeUint8(uint8_t val) { return write(&val, sizeof(val)); }
    bool        writeUint16(uint16_t val) { return write(&val, sizeof(val)); }
    bool        writeUint32(uint32_t val) { return write(&val, sizeof(val)); }
    bool        writeUint64(uint64_t val) { return write(&val, sizeof(val)); }

    bool        writeInt8(int8_t val) { return write(&val, sizeof(val)); }
    bool        writeInt16(int16_t val) { return write(&val, sizeof(val)); }
    bool        writeInt32(int32_t val) { return write(&val, sizeof(val)); }
    bool        writeInt64(int64_t val) { return write(&val, sizeof(val)); }

    bool        readUint8(uint8_t *pv) { return read(pv, sizeof(*pv)); }
    bool        readUint16(uint16_t *pv) { return read(pv, sizeof(*pv)); }
    bool        readUint32(uint32_t *pv) { return read(pv, sizeof(*pv)); }
    bool        readUint64(uint64_t *pv) { return read(pv, sizeof(*pv)); }

    bool        readInt8(int8_t *pv) { return read(pv, sizeof(*pv)); }
    bool        readInt16(int16_t *pv) { return read(pv, sizeof(*pv)); }
    bool        readInt32(int32_t *pv) { return read(pv, sizeof(*pv)); }
    bool        readInt64(int64_t *pv) { return read(pv, sizeof(*pv)); }

    std::string dump()  const;

private:
    size_t      calculate(size_t);
    size_t      allocBuffer(size_t size);
    void        freeBuffer();
    void        moveAssign(ByteBuffer &other);
    void        detach();

private:
    uint8_t*    mBuffer;
    size_t      mDataSize;
    size_t      mCapacity;
    size_t      mPos;
};

} // namespace eular

#endif // __EULAR_BUFFER_H__
