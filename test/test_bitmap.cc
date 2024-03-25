/*************************************************************************
    > File Name: test_bitmap.cc
    > Author: hsz
    > Brief:
    > Created Time: 2022-05-23 11:52:14 Monday
 ************************************************************************/

#define CATCH_CONFIG_MAIN

#include "catch/catch.hpp"
#include "../utils/include/bitmap.h"

TEST_CASE("test_constructer", "[bitmap]") {
    eular::BitMap bitMapObj1;
    REQUIRE(0 == bitMapObj1.count());

    eular::BitMap bitMapObj2(16);
    REQUIRE(16 == bitMapObj2.capacity());

    eular::BitMap fromCopy(bitMapObj2);
    REQUIRE(16 == fromCopy.capacity());
}

TEST_CASE("test_set_at", "[bitmap]") {
    eular::BitMap bitMapObj(16);

    const uint32_t count = 2;

    for (uint32_t i = 0; i < count; ++i) {
        bitMapObj.set(i, true);
    }

    REQUIRE(count == bitMapObj.count());
    REQUIRE_FALSE(bitMapObj.set(16, true));

    for (uint32_t i = 0; i < count; ++i) {
        REQUIRE(bitMapObj.at(i));
    }
}

TEST_CASE("test_reserve", "[bitmap]") {
    eular::BitMap bitMapObj(16);
    REQUIRE(16 == bitMapObj.capacity());

    bitMapObj.reserve(32);
    REQUIRE(32 == bitMapObj.capacity());
}

TEST_CASE("test_clear", "[bitmap]") {
    eular::BitMap bitMapObj(16);

    const uint32_t count = 2;

    for (uint32_t i = 0; i < count; ++i) {
        bitMapObj.set(i, true);
    }

    bitMapObj.clear();
    REQUIRE(0 == bitMapObj.count());
}
