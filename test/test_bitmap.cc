/*************************************************************************
    > File Name: test_bitmap.cc
    > Author: hsz
    > Brief:
    > Created Time: 2022-05-23 11:52:14 Monday
 ************************************************************************/

#include <utils/bitmap.h>
#include <gtest/gtest.h>
#include <iostream>
#include <assert.h>

TEST(BitMap_Test, test_constructer) {
    eular::BitMap bitMapObj1;
    EXPECT_TRUE(0 == bitMapObj1.count());

    eular::BitMap bitMapObj2(16);
    EXPECT_TRUE(16 == bitMapObj2.capacity());

    eular::BitMap fromCopy(bitMapObj2);
    EXPECT_TRUE(16 == fromCopy.capacity());
}

TEST(BitMap_Test, test_set_at) {
    eular::BitMap bitMapObj(16);

    const uint32_t count = 2;

    for (uint32_t i = 0; i < count; ++i) {
        bitMapObj.set(i, true);
    }

    EXPECT_TRUE(count == bitMapObj.count());
    EXPECT_FALSE(bitMapObj.set(16, true));

    for (uint32_t i = 0; i < count; ++i) {
        EXPECT_TRUE(bitMapObj.at(i));
    }
}

TEST(BitMap_Test, test_reserve) {
    eular::BitMap bitMapObj(16);
    EXPECT_TRUE(16 == bitMapObj.capacity());

    bitMapObj.reserve(32);
    EXPECT_TRUE(32 == bitMapObj.capacity());
}

TEST(BitMap_Test, test_clear) {
    eular::BitMap bitMapObj(16);

    const uint32_t count = 2;

    for (uint32_t i = 0; i < count; ++i) {
        bitMapObj.set(i, true);
    }

    bitMapObj.clear();
    EXPECT_TRUE(0 == bitMapObj.count());
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}