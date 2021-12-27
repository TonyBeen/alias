/*************************************************************************
    > File Name: test_aes.cc
    > Author: hsz
    > Brief:
    > Created Time: Mon 27 Dec 2021 01:35:15 PM CST
 ************************************************************************/

#include "aes.h"
#include <assert.h>
#include <iostream>

using namespace eular;
using namespace std;

int main(int argc, char **argv)
{
    uint8_t userKey[16] = {
        '1', '2', '3', '4', '5', '6',
        '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
    Aes aes(userKey, Aes::KeyType::AES128, Aes::EncodeType::AESCBC);
    uint8_t in[1024] = {0};
    uint8_t out[1024] = {0};
    uint8_t tmp[1024] = {0};

    printf("input one string:\n");
    scanf("%[^\n]", in);

    int encodeSize = aes.encode(out, in, strlen((char *)in));
    if (encodeSize < 0) {
        return -1;
    }
    printf("after encode: %d\n", encodeSize);
    for (int i = 0; i < encodeSize; ++i) {
        if (i != 0 && i % 16 == 0) {
            printf("\n");
        }
        printf("%02x ", out[i]);
    }
    printf("\n");

    int decodeSize = aes.decode(tmp, out, encodeSize);
    printf("after decrypt: %s\n", tmp);
    for (int i = 0; i < decodeSize; ++i) {
        if (i != 0 && i % 16 == 0) {
            printf("\n");
        }
        printf("%02x ", tmp[i]);
    }
    printf("\n");

    assert(memcmp(in, tmp, decodeSize) == 0);
    return 0;
}
