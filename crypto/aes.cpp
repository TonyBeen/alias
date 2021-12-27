/*************************************************************************
    > File Name: aes.cpp
    > Author: hsz
    > Brief: 对称加密，速度快于非对称加密
    > Created Time: Wed 22 Dec 2021 10:09:11 AM CST
 ************************************************************************/

#include "aes.h"
#include <utils/Errors.h>
#include <utils/exception.h>
#include <utils/string8.h>
#include <log/log.h>

#define LOG_TAG "AES"

namespace eular {

Aes::Aes(const uint8_t *userKey, Aes::KeyType userKeytype, Aes::EncodeType encodeType)
{
    memset(mUserKey, 0, MAX_USER_KEY_SIZE);
    memset(vecForCBC, 0, CBC_VECTOR_SIZE);
    reinit(userKey, userKeytype, encodeType);
}

Aes::~Aes()
{

}

bool Aes::reinit(const uint8_t *userKey, Aes::KeyType userKeytype, Aes::EncodeType encodeType)
{
    switch (userKeytype) {
    case Aes::KeyType::AES128:
    case Aes::KeyType::AES256:
        break;
    default:
        throw(Exception(String8::format("Invalid AES Type: %d", userKeytype)));
        break;
    }
    memcpy(mUserKey, userKey, userKeytype);
    mUserKeyType = userKeytype;
    mEncodeType = encodeType;
}

int Aes::encode(uint8_t *out, const uint8_t *src, const uint32_t &srcLen)
{
    LOG_ASSERT(out || src || srcLen, "");
    std::shared_ptr<ByteBuffer> ptr = PKCS7Padding(src, srcLen);
    if (ptr == nullptr) {
        return NO_MEMORY;
    }

    AES_set_encrypt_key(mUserKey, mUserKeyType * 8, &mAesKey);
    if (mEncodeType == AESECB) {
        AES_ecb_encrypt(ptr->const_data(), out, &mAesKey, AES_ENCRYPT);
    } else if (mEncodeType == AESCBC) {
        memset(vecForCBC, 0, AES_BLOCK_SIZE);   // 加密和解密时初始vecForCBC内容须一致，一般设置为全0
        AES_cbc_encrypt(ptr->const_data(), out, ptr->size(), &mAesKey, vecForCBC, AES_ENCRYPT);
    }

    return ptr->size();
}

int Aes::decode(uint8_t *out, const uint8_t *src, const uint32_t &srcLen)
{
    LOG_ASSERT(out || src || srcLen, "");

    AES_set_decrypt_key(mUserKey, mUserKeyType * 8, &mAesKey);
    if (mEncodeType == AESECB) {
        AES_ecb_encrypt(src, out, &mAesKey, AES_DECRYPT);
    } else if (mEncodeType == AESCBC) {
        memset(vecForCBC, 0, AES_BLOCK_SIZE);
        AES_cbc_encrypt(src, out, srcLen, &mAesKey, vecForCBC, AES_DECRYPT);
    }

    uint8_t paddingSize = 0;
    if (out[srcLen - 1] == 0x1) {    // 填充了一个
        paddingSize = 0x1;
    } else if (out[srcLen - 1] == out[srcLen - 2]) {
        paddingSize = out[srcLen - 1];
    }

    out[srcLen - paddingSize] = '\0';
    return srcLen - paddingSize;
}

std::shared_ptr<ByteBuffer> Aes::PKCS7Padding(const uint8_t *in, uint32_t inLen)
{
    std::shared_ptr<ByteBuffer> ptr(new ByteBuffer(in, inLen));
    uint8_t remainderSize = inLen % AES_BLOCK_SIZE;
    if (remainderSize == 0) {
        return ptr;
    }

    uint8_t buf[AES_BLOCK_SIZE] = {0};
    uint8_t paddingSize = AES_BLOCK_SIZE - remainderSize;

    memset(buf, paddingSize, paddingSize);
    if (ptr != nullptr) {
        ptr->append(buf, paddingSize);
    }

    return ptr;
}

} // namespace eular
