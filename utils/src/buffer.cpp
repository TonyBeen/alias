/*************************************************************************
    > File Name: Buffer.cpp
    > Author: hsz
    > Mail:
    > Created Time: Mon Jul  5 13:09:00 2021
 ************************************************************************/

// #define _DEBUG
#include "buffer.h"
#include "sharedbuffer.h"
#include "debug.h"
#include "exception.h"
#include <assert.h>

#define DEFAULT_BUFFER_SIZE (256)

namespace eular {
ByteBuffer::ByteBuffer() : ByteBuffer(DEFAULT_BUFFER_SIZE)
{
}

ByteBuffer::ByteBuffer(size_t size) :
    mBuffer(nullptr),
    mCapacity(0),
    mDataSize(0)
{
    mCapacity = getBuffer(size);
    LOG("%s(size_t size) mCapacity = %zu\n", __func__, mCapacity);
}

ByteBuffer::ByteBuffer(const char *data, size_t dataLength) :
    mBuffer(nullptr),
    mCapacity(0),
    mDataSize(0)
{
    mCapacity = getBuffer(dataLength + 1);
    set((const uint8_t *)data, dataLength);
    LOG("%s(const uint8_t *data, size_t dataLength) mDataSize = %zu, mCapacity = %zu\n",
        __func__, mDataSize, mCapacity);
}

ByteBuffer::ByteBuffer(const uint8_t *data, size_t dataLength) :
    mBuffer(nullptr),
    mCapacity(0),
    mDataSize(0)
{
    mCapacity = getBuffer(dataLength);
    set((const uint8_t *)data, dataLength);
}

ByteBuffer::ByteBuffer(const ByteBuffer& other) :
    mBuffer(other.mBuffer),
    mCapacity(other.mCapacity),
    mDataSize(other.mDataSize)
{
    SharedBuffer::bufferFromData(mBuffer)->acquire();

    // mCapacity = getBuffer(other.size());
    // set(other.mBuffer, other.mDataSize);
}

ByteBuffer::ByteBuffer(ByteBuffer&& other) :
    mBuffer(nullptr),
    mCapacity(0),
    mDataSize(0)
{
    if (&other == this) {
        return;
    }

    uint8_t *tmp = this->mBuffer;
    this->mBuffer = other.mBuffer;
    other.mBuffer = tmp;
    this->mDataSize = other.mDataSize;
    this->mCapacity = other.mCapacity;
}

ByteBuffer::~ByteBuffer()
{
    freeBuffer();
}

ByteBuffer& ByteBuffer::operator=(const ByteBuffer& other)
{
    if (&other == this) {
        return *this;
    }

    LOG("%s(const ByteBuffer& other)\n", __func__);
    mBuffer = other.mBuffer;
    mCapacity = other.mCapacity;
    mDataSize = other.mDataSize;
    SharedBuffer::bufferFromData(mBuffer)->acquire();
    return *this;
}

ByteBuffer& ByteBuffer::operator=(ByteBuffer&& other)
{
    if (&other == this) {
        return *this;
    }

    LOG("%s(ByteBuffer&& other) 移动赋值 mBuffer = %p, this: %s, other: %s\n",
        __func__, mBuffer, mBuffer, other.mBuffer);
    uint8_t *tmp = this->mBuffer;
    this->mBuffer = other.mBuffer;
    other.mBuffer = tmp;
    this->mDataSize = other.mDataSize;
    this->mCapacity = other.mCapacity;
    return *this;
}

uint8_t& ByteBuffer::operator[](size_t index)
{
    assert(index < mCapacity);
    if (mBuffer == nullptr) {
        mCapacity = getBuffer(DEFAULT_BUFFER_SIZE);
        if (mBuffer == nullptr) {
            throw Exception("no memory");
        }
    }

    return mBuffer[index];
}

size_t ByteBuffer::set(const uint8_t *data, size_t dataSize, size_t offset)
{
    if (data == nullptr || dataSize == 0) {
        return 0;
    }

    size_t real_offset = mDataSize >= offset ? offset : 0;
    uint8_t *temp = nullptr;
    size_t newSize = 0;

    if (__builtin_add_overflow(dataSize, real_offset, &newSize)) {
        return 0;
    }

    SharedBuffer *buf = nullptr;
    if (mCapacity < newSize) { // capacity exceeded
        buf = SharedBuffer::bufferFromData(mBuffer)->editResize(calculate(newSize));
    } else {
        buf = SharedBuffer::bufferFromData(mBuffer)->edit();
    }
    if (buf == nullptr) {
        return 0;
    }

    mBuffer = static_cast<uint8_t *>(buf->data());
    mCapacity = buf->size();
    mDataSize = newSize;
    memmove(mBuffer + real_offset, data, dataSize);

    LOG("%s() data(%p), dataSize(%zu), real_offset(%zu), mBuffer = %p, mCapacity = %zu mDataSize = %zu\n",
            __func__, data, dataSize, real_offset, mBuffer, mCapacity, mDataSize);

    return dataSize;
}

void ByteBuffer::append(const uint8_t *data, size_t dataSize)
{
    set(data, dataSize, size());
}

size_t ByteBuffer::insert(const uint8_t *data, size_t dataSize, size_t offset)
{
    if (data == nullptr || offset > mDataSize) { // offset范围必须在0-mDataSize，等于mDataSize相当于尾插，offset=0相当于头插
        return 0;
    }

    size_t newSize = 0;
    size_t copySize = mDataSize - offset;
    size_t oldDataSize = mDataSize;
    if (__builtin_add_overflow(dataSize, mDataSize, &newSize)) {
        return 0;
    }

    SharedBuffer *buf = nullptr;
    if (mCapacity < newSize) {
        buf = SharedBuffer::bufferFromData(mBuffer)->editResize(calculate(newSize));
    } else {
        buf = SharedBuffer::bufferFromData(mBuffer)->edit();
    }

    if (buf == nullptr) {
        return 0;
    }

    mBuffer = static_cast<uint8_t *>(buf->data());
    mCapacity = buf->size();
    mDataSize = newSize;

    if (copySize == 0) {
        memmove(mBuffer + oldDataSize, data, dataSize);
    } else {
        uint8_t *temp = (uint8_t *)malloc(copySize);
        if (temp == nullptr) {
            return 0;
        }
        memmove(temp, mBuffer + offset, copySize);
        memmove(mBuffer + offset, data, dataSize);
        memmove(mBuffer + offset + dataSize, temp, copySize);
        free(temp);
    }

    return dataSize;
}

void ByteBuffer::resize(size_t newSize)
{
    if (newSize == 0) {
        return;
    }

    SharedBuffer *buf = SharedBuffer::bufferFromData(mBuffer)->editResize(newSize);
    if (buf) {
        mBuffer = static_cast<uint8_t *>(buf->data());
        mCapacity = buf->size();
    }
}

void ByteBuffer::clear() 
{
    if (mBuffer) {
        memset(mBuffer, 0, mCapacity);
        mDataSize = 0;
    }
}

std::string ByteBuffer::dump() const
{
    std::string ret;
    char buf[64] = {0};
    int cycle = mDataSize / 4;
    for (int i = 0; i < cycle; ++i) {
        snprintf(buf, sizeof(buf), "0x%02x 0x%02x 0x%02x 0x%02x ",
            mBuffer[i * 4], mBuffer[i * 4 + 1], mBuffer[i * 4 + 2], mBuffer[i * 4 + 3]);
        ret.append(buf);
    }

    cycle = mDataSize % 4;
    for (int i = 0; i < cycle; ++i) {
        snprintf(buf, sizeof(buf), "0x%02x ", mBuffer[i]);
        ret.append(buf);
    }

    return ret;
}

size_t ByteBuffer::calculate(size_t dataSize)
{
    if (dataSize >= DEFAULT_BUFFER_SIZE) {
        dataSize *= 1.5;
    } else {
        dataSize = DEFAULT_BUFFER_SIZE;
    }
    return dataSize;
}

size_t ByteBuffer::getBuffer(size_t size)
{
    if (size == 0) {
        size = DEFAULT_BUFFER_SIZE;
    }

    if (mBuffer == nullptr) {
        mBuffer = static_cast<uint8_t *>(SharedBuffer::alloc(size)->data());
        LOG("new buffer %p\n", mBuffer);
        if (mBuffer) {
            memset(mBuffer, 0, size);
            return size;
        }
    }

    return 0;
}

void ByteBuffer::freeBuffer()
{
    LOG("%s() %p\n", __func__, mBuffer);
    if (mBuffer) {
        SharedBuffer::bufferFromData(mBuffer)->release();
    }
    mBuffer = nullptr;
    mDataSize = 0;
    mCapacity = 0;
}

} // namespace eular
