/*************************************************************************
    > File Name: buffer_stream.h
    > Author: hsz
    > Brief:
    > Created Time: 2024年04月25日 星期四 08时58分10秒
 ************************************************************************/

#ifndef __EULAR_BUFFER_STREAM_H__
#define __EULAR_BUFFER_STREAM_H__

#include <memory>

#include <utils/exception.h>
#include <utils/buffer.h>

namespace eular {
class BufferStream
{
public:
    typedef std::shared_ptr<BufferStream>   SP;
    typedef std::weak_ptr<BufferStream>     WP;

    BufferStream();
    BufferStream(ByteBuffer &buffer);
    BufferStream(const BufferStream &) = delete;
    BufferStream(BufferStream &&other);
    ~BufferStream();

    BufferStream&operator=(const BufferStream &) = delete;
    BufferStream&operator=(BufferStream &&other);

    BufferStream &operator<<(bool item);
    BufferStream &operator<<(int8_t item);
    BufferStream &operator<<(uint8_t item);
    BufferStream &operator<<(wchar_t item);
    BufferStream &operator<<(int16_t item);
    BufferStream &operator<<(uint16_t item);
    BufferStream &operator<<(float item);
    BufferStream &operator<<(int32_t item);
    BufferStream &operator<<(uint32_t item);
    BufferStream &operator<<(double item);
    BufferStream &operator<<(int64_t item);
    BufferStream &operator<<(uint64_t item);
    BufferStream &operator<<(const std::string &item);

    BufferStream &operator>>(bool &item);
    BufferStream &operator>>(int8_t &item);
    BufferStream &operator>>(uint8_t &item);
    BufferStream &operator>>(wchar_t &item);
    BufferStream &operator>>(int16_t &item);
    BufferStream &operator>>(uint16_t &item);
    BufferStream &operator>>(float &item);
    BufferStream &operator>>(int32_t &item);
    BufferStream &operator>>(uint32_t &item);
    BufferStream &operator>>(double &item);
    BufferStream &operator>>(int64_t &item);
    BufferStream &operator>>(uint64_t &item);
    BufferStream &operator>>(std::string &item);

    void write(const void *data, uint32_t size);
    bool read(void *data, uint32_t size);

private:
    void checkBuffer() const;

private:
    ByteBuffer*     m_buffer;   // 缓存
    uint32_t        m_rpos;     // 读索引
    uint32_t        m_wpos;     // 写索引
};

} // namespace eular

#endif // __EULAR_BUFFER_STREAM_H__
