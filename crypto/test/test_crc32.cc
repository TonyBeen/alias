/*************************************************************************
    > File Name: test_crc32.cc
    > Author: hsz
    > Brief:
    > Created Time: Thu 20 Jul 2023 10:53:34 AM CST
 ************************************************************************/

#define DEBUG_SELF

#ifdef DEBUG_SELF
#include "crc32.h"
#else 
#include "zlib.h" // -lz
#endif
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <gtest/gtest.h>

#define CRC32_RESULT 0xdb8976d7UL

#ifdef DEBUG_SELF
TEST(SelfCRC32, test_crc32) {
    uint64_t crc1 = crc32(0L, (const uint8_t*)"Hello,", 6);
    uint64_t crc2 = crc32(0L, (const uint8_t*)"World!", 6);

    uint64_t len2 = 6; // 第二个 CRC32 对应的数据长度
    uint64_t combinedCrc = crc32_combine64(crc1, crc2, len2);

    uint64_t crc3 = crc32(0L, (const uint8_t*)"Hello,World!", 12);

    EXPECT_EQ(combinedCrc, crc3);
}
#else 
uint64_t test_crc32()
{
    uLong crc1 = crc32(0L, (const Bytef*)"Hello,", 6);
    uLong crc2 = crc32(0L, (const Bytef*)"World!", 6);

    z_off64_t len2 = 6; // 第二个 CRC32 对应的数据长度

    uLong combinedCrc = crc32_combine64(crc1, crc2, len2);

    EXPECT_EQ(combinedCrc, CRC32_RESULT);
}
#endif
