/*************************************************************************
    > File Name: test_sharedbuffer.cc
    > Author: hsz
    > Brief:
    > Created Time: Mon 12 Dec 2022 03:18:41 PM CST
 ************************************************************************/

#define __STDC_LIMIT_MACROS

#include <utils/sharedbuffer.h>
#include <gtest/gtest.h>
#include <memory>
#include <stdint.h>

extern "C" void __hwasan_init() __attribute__((weak));
#define SKIP_WITH_HWASAN \
    if (&__hwasan_init != 0) GTEST_SKIP()

TEST(SharedBufferTest, alloc_death) {
    EXPECT_DEATH(eular::SharedBuffer::alloc(SIZE_MAX), "");
    EXPECT_DEATH(eular::SharedBuffer::alloc(SIZE_MAX - sizeof(eular::SharedBuffer)), "");
}

TEST(SharedBufferTest, alloc_max) {
    SKIP_WITH_HWASAN;  // hwasan has a 2GiB allocation limit.

    eular::SharedBuffer* buf =
            eular::SharedBuffer::alloc(SIZE_MAX - sizeof(eular::SharedBuffer) - 1);
    if (buf != nullptr) {
        EXPECT_NE(nullptr, buf->data());
        buf->release();
    }
}

TEST(SharedBufferTest, alloc_big) {
    SKIP_WITH_HWASAN;  // hwasan has a 2GiB allocation limit.

    eular::SharedBuffer* buf = eular::SharedBuffer::alloc(SIZE_MAX / 2);
    if (buf != nullptr) {
        EXPECT_NE(nullptr, buf->data());
        buf->release();
    }
}

TEST(SharedBufferTest, alloc_zero_size) {
    eular::SharedBuffer* buf = eular::SharedBuffer::alloc(0);
    ASSERT_NE(nullptr, buf);
    ASSERT_EQ(0U, buf->size());
    buf->release();
}

TEST(SharedBufferTest, editResize_death) {
    eular::SharedBuffer* buf = eular::SharedBuffer::alloc(10);
    EXPECT_DEATH(buf->editResize(SIZE_MAX - sizeof(eular::SharedBuffer)), "");
    buf = eular::SharedBuffer::alloc(10);
    EXPECT_DEATH(buf->editResize(SIZE_MAX), "");
}

TEST(SharedBufferTest, editResize_null) {
    // Big enough to fail, not big enough to abort.
    SKIP_WITH_HWASAN;  // hwasan has a 2GiB allocation limit.
    eular::SharedBuffer* buf = eular::SharedBuffer::alloc(10);
    eular::SharedBuffer* buf2 = buf->editResize(SIZE_MAX / 2);
    if (buf2 == nullptr) {
        buf->release();
    } else {
        EXPECT_NE(nullptr, buf2->data());
        buf2->release();
    }
}

TEST(SharedBufferTest, editResize_zero_size) {
    eular::SharedBuffer* buf = eular::SharedBuffer::alloc(10);
    buf = buf->editResize(0);
    ASSERT_EQ(0U, buf->size());
    buf->release();
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}
