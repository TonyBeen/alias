/*************************************************************************
    > File Name: test_base64.cc
    > Author: hsz
    > Brief:
    > Created Time: Tue 28 Dec 2021 09:50:44 AM CST
 ************************************************************************/

#include "base64.h"
#include <utils/buffer.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <gtest/gtest.h>

using namespace eular;
using namespace std;

TEST(Base64Test, test_encode_docode) {
    const char *file = "for_base64_test.txt"; 

    ByteBuffer buf(4096);
    uint8_t from[2048] = {0};
    uint8_t out[2048] = {0};

    int fd = open(file, O_RDONLY);
    if (fd < 0) {
        perror("open error");
        return;
    }

    while (1) {
        int readSize = read(fd, from, 2048);
        if (readSize > 0) {
            buf.append(from, readSize);
        } else if (readSize == 0) {
            break;
        } else {
            perror("read error");
            return;
        }
        memset(from, 0, 2048);
    }

    Base64 base64;
    int encodeLen = base64.encode(out, buf.const_data(), buf.size());
    int decodeLen = base64.decode(from, out, encodeLen);
    ASSERT_EQ(decodeLen, buf.size());
    EXPECT_EQ(memcmp(buf.const_data(), from, decodeLen), 0);
}
