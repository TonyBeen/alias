/*************************************************************************
    > File Name: test_rsa.cc
    > Author: hsz
    > Brief:
    > Created Time: Fri 24 Dec 2021 11:25:20 AM CST
 ************************************************************************/

#include "rsa.h"

using namespace eular;

static const char *publicKeyFile = "public.key";
static const char *privateKeyFile = "private.key";

// 测试公钥加密，私钥解密
void test_publicEncode_privateDecode()
{
    Rsa rsa(publicKeyFile, privateKeyFile);

    const int bufSize = 1024;
    unsigned char from[bufSize] = {0};
    unsigned char out[bufSize] = {0};
    unsigned char tmp[bufSize] = {0};

    printf("something you want to encrypt (at most 512, prevent overflow):\n");
    scanf("%[^\n]", from);
    printf("the thing you input: [%s]\n", from);

    printf("pubsize = %d, prisize = %d\n", rsa.getPubRsaSize(), rsa.getPriRsaSize());
    printf("neededEncodeSize = %u\n", rsa.getEncodeSpaceByDataLen(strlen((char *)from), false));
    int encodeLen = rsa.publicEncode(out, from, strlen((char *)from));
    if (encodeLen < 0) {
        printf("encode failed\n");
        return;
    }
    printf("after encode: encodeLen = %d\n", encodeLen);
    for (int i = 0; i < encodeLen; ++i) {
        if (i != 0 && i % 16 == 0) {
            printf("\n");
        }
        printf("%02x ", out[i]);
    }
    printf("\n");

    printf("begin decode...\n");
    printf("neededDecodeSize = %u\n", rsa.getDecodeSpaceByDataLen(encodeLen, true));
    memset(from, bufSize, 0);
    memcpy(from, out, encodeLen);
    memset(out, 0, sizeof(out));
    int decodeLen = rsa.privateDecode(out, from, encodeLen);
    printf("decodeLen = %d\n", decodeLen);
    printf("%s\n", (char *)out);
}

// 测试私钥加密，公钥解密
void test_privateEncode_publicDecode()
{
    Rsa rsa(publicKeyFile, privateKeyFile);

    const int bufSize = 1024;
    unsigned char from[bufSize] = {0};
    unsigned char out[bufSize] = {0};
    unsigned char tmp[bufSize] = {0};

    printf("something you want to encrypt:\n");
    scanf("%s", from);  // 去除stdin多余的字符
    scanf("%[^\n]", from + strlen((char *)from)); //fscanf(stdin, "%[^\n]", from);
    printf("the thing you input: [%s] [len = %zu]\n", from, strlen((char *)from));

    printf("pubsize = %d, prisize = %d\n", rsa.getPubRsaSize(), rsa.getPriRsaSize());

    int encodeSize = rsa.privateEncode(out, from, strlen((char *)from));
    if (encodeSize < 0) {
        printf("encode failed\n");
        return;
    }
    printf("after encode: encodeSize = %d\n", encodeSize);
    for (int i = 0; i < encodeSize; ++i) {
        if (i != 0 && i % 16 == 0) {
            printf("\n");
        }
        printf("%02x ", out[i]);
    }
    printf("\n");  

    memset(from, 0, bufSize);
    memcpy(from, out, bufSize);
    memset(out, 0, bufSize);
    int decodeSize = rsa.publicDecode(out, from, encodeSize);
    printf("decodeSize = %d\n", decodeSize);
    printf("{%s}\n", out);
}


int main()
{
    if (Rsa::GenerateKey(publicKeyFile, privateKeyFile) < 0) {
        return -1;
    }
    
    test_publicEncode_privateDecode();
    test_privateEncode_publicDecode();
    return 0;
}