/*************************************************************************
    > File Name: base64.cpp
    > Author: hsz
    > Brief:
    > Created Time: Tue 21 Dec 2021 02:32:59 PM CST
 ************************************************************************/

#include "base64.h"
#include <utils/Errors.h>
#include <assert.h>

namespace eular {
Base64::Base64()
{
    init();    
}

Base64::~Base64()
{
    destroy();
}

bool Base64::init()
{
    if (mEncodeCtx && mDecodeCtx) {
        return true;
    }
    if (mEncodeCtx == nullptr) {
        mEncodeCtx = EVP_ENCODE_CTX_new();
        EVP_EncodeInit(mEncodeCtx);
    }
    if (mDecodeCtx == nullptr) {
        mDecodeCtx = EVP_ENCODE_CTX_new();
        EVP_DecodeInit(mDecodeCtx);
    }

    EVP_EncodeSetFlag(mEncodeCtx, 1);   // 去掉openssl默认每48字节加一个换行符，自己加的函数
    return (mEncodeCtx && mDecodeCtx);
}

bool Base64::destroy()
{
    if (mEncodeCtx) {
        EVP_ENCODE_CTX_free(mEncodeCtx);
        mEncodeCtx = nullptr;
    }
    if (mDecodeCtx) {
        EVP_ENCODE_CTX_free(mDecodeCtx);
        mDecodeCtx = nullptr;
    }
    return true;
}

uint32_t Base64::GetSpaceSize(const uint32_t &len, bool isEncode)
{
    // uint32_t baseSize = 0;
    // if (len > 0) {
    //     baseSize = len * 8 / 6 + len / 48;  // 加上len/48是因为每48字节加一个\n
    //     switch (len % 3)
    //     {
    //     case 0:
    //         baseSize += 1;  // 结束符\n
    //         break;
    //     case 1:
    //         baseSize += 4;  // 补充的2个00(1byte) + ==(2byte) + 结束符\n
    //         break;
    //     case 2:
    //         baseSize += 3;  // 补充的1个00(1byte) + =(1byte) + 结束符\n
    //         break;
    //     default:
    //         assert(0); // will never happen;
    //         break;
    //     }
    // }

    // return baseSize + 1;    // 防止溢出
    return isEncode ? EVP_ENCODE_LENGTH(len) : EVP_DECODE_LENGTH(len);
}

void Base64::SetEncodeFlag(uint8_t noNewLine)
{
    if (mEncodeCtx) {
        EVP_EncodeSetFlag(mEncodeCtx, noNewLine);
    }
}

// 可以编码大小写字母，数字，--=+/*%@$#()/.,:# !
int Base64::encode(uint8_t *out, const uint8_t *src, const uint32_t &srcLen)
{
    if (mEncodeCtx == nullptr) {
        return NO_INIT;
    }
    if (src == nullptr || srcLen == 0) {
        return INVALID_PARAM;
    }

    int32_t outLen, total;
    // openssl base64编码以48字节为一单位，以48*n个字节进行编码，剩余的字节数(小于48)则放入EVP_ENCODE_CTX::enc_data[48],
    // 为EVP_ENCODE_CTX::num赋值为剩余字节数
    EVP_EncodeUpdate(mEncodeCtx, out, &outLen, src, srcLen);
    total = outLen;
    // 此函数作用是处理EVP_ENCODE_CTX::enc_data中的数据
    EVP_EncodeFinal(mEncodeCtx, out + total, &outLen);
    total += outLen;
    return total;
}

int Base64::decode(uint8_t *out, const uint8_t *src, const uint32_t &srcLen)
{
    if (mDecodeCtx == nullptr) {
        return NO_INIT;
    }
    if (src == nullptr || srcLen == 0) {
        return INVALID_PARAM;
    }

    int32_t outLen, total;
    EVP_DecodeUpdate(mDecodeCtx, out, &outLen, src, srcLen);
    total = outLen;
    EVP_DecodeFinal(mDecodeCtx, out + total, &outLen);
    total += outLen;
    return total;
}

} // namespace eular
