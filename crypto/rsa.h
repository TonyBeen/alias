/*************************************************************************
    > File Name: rsa.h
    > Author: hsz
    > Brief:
    > Created Time: Wed 22 Dec 2021 10:10:02 AM CST
 ************************************************************************/

#ifndef __CRYPTO_RSA_H__
#define __CRYPTO_RSA_H__

#include "crypto.h"
#include <utils/Buffer.h>
#include <utils/string8.h>
#include <openssl/rsa.h>

namespace eular {
class Rsa
{
public:
    Rsa(const String8 &pubkeyFile, const String8 &prikeyFile);
    virtual ~Rsa();

    static int GenerateKey(const String8 &pubkeyFile, const String8 &prikeyFile, uint32_t len = 1024);

    // 使用公钥加密
    int publicEncode(uint8_t *out, const uint8_t *src, uint32_t srcLen);
    // 使用公钥解密
    int publicDecode(uint8_t *out, const uint8_t *src, uint32_t srcLen);
    // 使用私钥加密
    int privateEncode(uint8_t *out, const uint8_t *src, uint32_t srcLen);
    // 使用私钥解密
    int privateDecode(uint8_t *out, const uint8_t *src, uint32_t srcLen);

    // 根据秘钥或公钥获得单位加密/解密字节数，受padding影响
    int getPubRsaSize() const { return RSA_size(mPublicKey); }
    int getPriRsaSize() const { return RSA_size(mPrivatKey); }

private:
    bool reinit();
    void destroy();

protected:
    RSA *mPublicKey;
    RSA *mPrivatKey;

    String8 mPubKeyPath;
    String8 mPriKeyPath;
    String8 mPubKeyStr;
    String8 mPriKeyStr;
    static const int defaultPadding = RSA_PKCS1_PADDING;
};

} // namespace eular

#endif // __CRYPTO_RSA_H__