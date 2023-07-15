#include <utils/buffer.h>
#include <utils/utils.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <bits/move.h>
#include <assert.h>
#include <gtest/gtest.h>

using namespace eular;

class BufferTest : public testing::Test {
public:
    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};

TEST_F(BufferTest, testConstructer) {
    {
        ByteBuffer buffer("Hello", 5);
        EXPECT_EQ(buffer.size(), 5);
    }

    {
        const uint8_t *data = (const uint8_t *)"Hello";
        ByteBuffer buffer(data, 5);
        EXPECT_EQ(buffer.size(), 5);
    }

    {
        ByteBuffer buffer(128);
        EXPECT_EQ(buffer.capacity(), 128);
    }

    {
        ByteBuffer buffer(128);
        ByteBuffer buf = std::move(buffer);
        const uint8_t *data = (const uint8_t *)"Hello";
        buffer.append(data, 5);
        EXPECT_EQ(buffer.size(), 5);
    }
}

TEST_F(BufferTest, testApendInsert) {
    ByteBuffer buffer(128);
    const uint8_t *data = (const uint8_t *)"Hello";
    buffer.append(data, 5);
    buffer.insert((const uint8_t *)"***", 3, 1);
    EXPECT_STREQ((char *)buffer.data(), "H***ello");
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}