/*************************************************************************
    > File Name: base64.h
    > Author: hsz
    > Brief: base64 encode & decode
    > Created Time: Tue 21 Dec 2021 02:32:55 PM CST
 ************************************************************************/

#ifndef __BASE64_H__
#define __BASE64_H__

#include "crypto.h"
#include <openssl/evp.h>

namespace Jarvis {
class Base64 : public CryptoBase
{
public:
    Base64();
    virtual ~Base64();

    bool init();
    bool destroy();

    /**
     * @brief 获取编码输出缓存所需的大小
     * @param len 将要编码的长度
     * @param isEncode 获取编码输出长度为true，解码为false，默认true
     * @return 成功返回大于0，失败返回0
     */
    static uint32_t GetSpaceSize(const uint32_t &len, bool isEncode = true);

    /**
     * @brief 1为不采用换行
     * @param noNewLine 取值0||1
     */
    void SetEncodeFlag(uint8_t noNewLine = 1);

    /**
     * @brief 对src进行编码，输出至out
     * @param out 编码输出缓存
     * @param src 要进行编码的缓存
     * @param srcLen src的长度
     * @return 失败返回负值，成功返回已编码的长度
     */
    virtual int encode(uint8_t *out, uint8_t *src, const uint32_t &srcLen) override;

    /**
     * @brief 对src进行解码，输出至out
     * @param out 解码输出缓存
     * @param src 要进行解码的缓存
     * @param srcLen src的长度
     * @return 失败返回负值，成功返回已编码的长度
     */
    virtual int decode(uint8_t *out, uint8_t *src, const uint32_t &srcLen) override;

protected:
    EVP_ENCODE_CTX *mEncodeCtx;
    EVP_ENCODE_CTX *mDecodeCtx;
};


} // namespace Jarvis

#endif // __BASE64_H__
