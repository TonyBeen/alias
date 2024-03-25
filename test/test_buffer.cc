#define CATCH_CONFIG_MAIN
#include <bits/move.h>

#include "catch/catch.hpp"
#include "../utils/include/buffer.h"
#include "../utils/include/utils.h"

using namespace eular;

TEST_CASE("test_Constructer", "[ByteBuffer]") {
    {
        ByteBuffer defaultBuffer;
        CHECK(defaultBuffer.size() == 0);
        CHECK(defaultBuffer.capacity() == 0);
    }

    {
        ByteBuffer buffer(128);
        CHECK(buffer.capacity() == 128);
    }

    {
        ByteBuffer buffer("Hello");
        CHECK(buffer.size() == 5);

        ByteBuffer buf(nullptr);
        CHECK(buf.size() == 0);
        CHECK(buf.capacity() == 0);
    }

    {
        const uint8_t *data = (const uint8_t *)"Hello";
        ByteBuffer buffer(data, 5);
        CHECK(buffer.size() == 5);
    }

    {
        ByteBuffer buffer(128);
        ByteBuffer buf = buffer;
        CHECK(buf.const_data() == buffer.const_data());
    }

    {
        ByteBuffer buffer;
        ByteBuffer buf;
        buf = buffer;
        CHECK(buf.const_data() == buffer.const_data());
    }

    {
        ByteBuffer buffer(128);
        ByteBuffer buf = std::move(buffer);
        CHECK(buffer.capacity() == 0);
        buffer.append("Hello");
        CHECK(buffer.size() == 5);
    }
}

TEST_CASE("test_apend_insert", "[ByteBuffer]") {
    ByteBuffer buffer(128);
    const uint8_t *data = (const uint8_t *)"Hello";
    buffer.append(data, 5);
    buffer.insert((const uint8_t *)"***", 3, 1);
    CHECK(std::string((char *)buffer.data()) == "H***ello");
}
