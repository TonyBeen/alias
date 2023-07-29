/*************************************************************************
    > File Name: aes.h
    > Author: hsz
    > Brief:
    > Created Time: Wed 22 Dec 2021 10:09:08 AM CST
 ************************************************************************/

#ifndef __CRYPTO_AES_H__
#define __CRYPTO_AES_H__

#include "crypto.h"
#include <utils/buffer.h>
#include <openssl/aes.h>
#include <memory>

#define MAX_USER_KEY_SIZE 32
#define CBC_VECTOR_SIZE 16

namespace eular {
class Aes : public CryptoBase
{
public:
    enum KeyType {
        AES128 = 16,
        AES256 = 32,
    };

    enum EncodeType {
        AESECB = 0,
        AESCBC = 1,
    };

    Aes();
    Aes(const uint8_t *userKey, uint32_t userKeyLen, Aes::KeyType userKeytype, Aes::EncodeType encodeType);
    virtual ~Aes();

    bool reinit(const uint8_t *userKey, uint32_t userKeyLen, Aes::KeyType userKeytype, Aes::EncodeType encodeType);

    uint8_t getKeyType() const { return mUserKeyType; }
    void setKeyType(uint8_t keyType) { mUserKeyType = keyType; }

    uint8_t getEncodeType() const { return mEncodeType; }
    void setEncodeType(uint8_t encodeType) { mEncodeType = encodeType; }

    const uint8_t *getKey() const { return mUserKey; }
    void setKey(const uint8_t *key, uint32_t len);

    // int32_t getEncodeLength() const;
    // int32_t getDecodeLength() const;

    int32_t encode(uint8_t *out, const uint8_t *src, const uint32_t &srcLen) override;
    int32_t decode(uint8_t *out, const uint8_t *src, const uint32_t &srcLen) override;

protected:
    std::shared_ptr<ByteBuffer> PKCS7Padding(const uint8_t *in, uint32_t inLen);

protected:
    AES_KEY mAesKey;
    uint8_t mUserKey[MAX_USER_KEY_SIZE];
    uint8_t mUserKeyType;
    uint8_t mEncodeType;
    uint8_t vecForCBC[CBC_VECTOR_SIZE];
};

} // namespace eular

#endif // __CRYPTO_AES_H__