/*************************************************************************
    > File Name: Buffer.cpp
    > Author: hsz
    > Mail:
    > Created Time: Mon Jul  5 13:09:00 2021
 ************************************************************************/

// #define _DEBUG
#include "Buffer.h"
#include "debug.h"
#include <assert.h>

#define DEFAULT_BUFFER_SIZE (4096)

namespace eular {
ByteBuffer::ByteBuffer() : ByteBuffer(DEFAULT_BUFFER_SIZE)
{
}

ByteBuffer::ByteBuffer(size_t size) :
    mDataSize(0),
    mBuffer(nullptr)
{
    mCapacity = getBuffer(size);
    LOG("%s(size_t size) mCapacity = %zu\n", __func__, mCapacity);
}

ByteBuffer::ByteBuffer(const char *data, size_t dataLength) :
    mBuffer(nullptr),
    mCapacity(0),
    mDataSize(0)
{
    set((const uint8_t *)data, dataLength);
    LOG("%s(const uint8_t *data, size_t dataLength) mDataSize = %zu, mCapacity = %zu\n",
        __func__, mDataSize, mCapacity);
}

ByteBuffer::ByteBuffer(const uint8_t *data, size_t dataLength) :
    mBuffer(nullptr),
    mCapacity(0),
    mDataSize(0)
{
    set((const uint8_t *)data, dataLength);
}

ByteBuffer::ByteBuffer(const ByteBuffer& other)
{
    if (&other == this) {   // is that possible?
        return;
    }
    
    set(other.mBuffer, other.mDataSize);
}

ByteBuffer::ByteBuffer(ByteBuffer&& other) :
    mBuffer(nullptr)
{
    LOG("移动构造 ByteBuffer::ByteBuffer(ByteBuffer&& other)\n");
    uint8_t *tmp = this->mBuffer;
    this->mBuffer = other.mBuffer;
    other.mBuffer = tmp;
}

ByteBuffer::~ByteBuffer()
{
    freeBuffer();
}

ByteBuffer& ByteBuffer::operator=(const ByteBuffer& other)
{
    LOG("%s(const ByteBuffer& other)\n", __func__);
    set(other.mBuffer, other.mDataSize);
    return *this;
}

ByteBuffer& ByteBuffer::operator=(ByteBuffer& other)
{
    LOG("%s(ByteBuffer& other)\n", __func__);
    set(other.mBuffer, other.mDataSize);
    return *this;
}

ByteBuffer& ByteBuffer::operator=(ByteBuffer&& other)
{
    LOG("%s(ByteBuffer&& other) 移动赋值 mBuffer = %p\n", __func__, mBuffer);
    uint8_t *tmp = this->mBuffer;
    this->mBuffer = other.mBuffer;
    other.mBuffer = tmp;
    return *this;
}

uint8_t& ByteBuffer::operator[](size_t index)
{
    assert(index < mCapacity);
    return mBuffer[index];
}

size_t ByteBuffer::set(const uint8_t *data, size_t dataSize, size_t offset)
{
    if (data == nullptr) {
        return 0;
    }
    uint8_t *temp = nullptr;
    if (mCapacity < (dataSize + offset)) {
        if (offset > 0) {
            temp = (uint8_t *)malloc(offset);
            if (temp == nullptr) {
                return 0;
            }
            memcpy(temp, mBuffer, offset);
        }

        freeBuffer();
        mCapacity = getBuffer(calculate(dataSize + offset));
    }
    LOG("ByteBuffer::%s() data(%s), dataSize(%zu), offset(%zu), mBuffer = %p, mCapacity = %zu mDataSize = %zu\n",
            __func__, data, dataSize, offset, mBuffer, mCapacity, mDataSize);

    if (mCapacity > 0) {
        if (temp) {
            memcpy(mBuffer, temp, offset);
            free(temp);
            temp = nullptr;
        }
        memset(mBuffer + offset, 0, mCapacity - offset); // 清空offset之后的数据
        memcpy(mBuffer + offset, data, dataSize);
        mDataSize = offset + dataSize;
        return dataSize;
    }
    if (temp) {
        free(temp);
        temp = nullptr;
    }
    return 0;
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
    uint8_t *temp = nullptr;
    if (mCapacity < (dataSize + mDataSize)) {
        temp = (uint8_t *)malloc(mDataSize);
        if (temp == nullptr) {
            return 0;
        }
        memcpy(temp, mBuffer, mDataSize);

        freeBuffer();
        mCapacity = getBuffer(calculate(dataSize + offset));
    }

    if (mCapacity > 0) {
        if (temp) {
            memcpy(mBuffer, temp, offset);
            memcpy(mBuffer + offset + dataSize, temp + offset, mDataSize - offset);
            free(temp);
        } else {
            memmove(mBuffer + offset + dataSize, mBuffer + offset, mDataSize - offset);
        }
        memcpy(mBuffer + offset, data, dataSize);
        mDataSize += dataSize;
        return dataSize;
    }
    // 说明getBuffer失败了
    if (temp) {
        free(temp);
    }

    return 0;
}

void ByteBuffer::resize(size_t newSize)
{
    if (newSize == 0) {
        return;
    }
    mCapacity = newSize;
    ByteBuffer tmp(mBuffer, mDataSize);
    freeBuffer();
    size_t realCap = getBuffer(newSize);
    if (realCap > 0) {
        *this = tmp;
    }
}

void ByteBuffer::clear() 
{
    if (mBuffer) {
        memset(mBuffer, 0, mCapacity);
        mDataSize = 0;
    }
}

size_t ByteBuffer::calculate(size_t dataSize)
{
    if (dataSize >= DEFAULT_BUFFER_SIZE) {
        dataSize *= 2;
    } else {
        dataSize = DEFAULT_BUFFER_SIZE;
    }
    return dataSize;
}

size_t ByteBuffer::getBuffer(size_t size = DEFAULT_BUFFER_SIZE)
{
    if (size == 0) {
        size = DEFAULT_BUFFER_SIZE;
    }
    if (mBuffer == nullptr) {
        mBuffer = (uint8_t *)malloc(size);
        LOG("new buffer %p\n", mBuffer);
        if (mBuffer) {
            memset(mBuffer, 0, size);
            return size;
        }
    }
    return 0;
}

void  ByteBuffer::freeBuffer()
{
    LOG("free() %p\n", mBuffer);
    if (mBuffer) {
        free(mBuffer);
        LOG("free %p\n", mBuffer);
    }
    mBuffer = nullptr;
}

} // namespace eular
