/*************************************************************************
    > File Name: md5.cpp
    > Author: hsz
    > Brief:
    > Created Time: Wed 22 Dec 2021 10:07:24 AM CST
 ************************************************************************/

#include "md5.h"


namespace eular {

Md5::Md5()
{
    MD5_Init(&mMd5);
}

Md5::~Md5()
{

}

int Md5::encode(uint8_t *out, const uint8_t *src, const uint32_t &srcLen)
{
    MD5_Update(&mMd5, src, srcLen);
    MD5_Final(out, &mMd5);

    return 16;
}

int Md5::decode(uint8_t *out, const uint8_t *src, const uint32_t &srcLen)
{
    return -1;
}


} // namespace eular
