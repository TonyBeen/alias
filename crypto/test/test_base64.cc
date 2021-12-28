/*************************************************************************
    > File Name: test_base64.cc
    > Author: hsz
    > Brief:
    > Created Time: Tue 28 Dec 2021 09:50:44 AM CST
 ************************************************************************/

#include "base64.h"
#include <utils/Buffer.h>
#include <log/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define LOG_TAG "test base64"

using namespace eular;
using namespace std;

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("usage: %s file\n", argv[0]);
        return -1;
    }

    const char *file = argv[1]; 

    ByteBuffer buf;
    uint8_t from[2048] = {0};
    uint8_t out[2048] = {0};

    int fd = open(file, O_RDONLY);
    if (fd < 0) {
        perror("open error");
        return -1;
    }
    LOGD("open success");
    while (1) {
        int readSize = read(fd, from, 2048);
        LOGD("read size = %d", readSize);
        if (readSize > 0) {
            buf.append(from, readSize);
        } else if (readSize == 0) {
            break;
        } else {
            perror("read error");
            return -1;
        }
        memset(from, 0, 2048);
    }

    Base64 base64;
    LOGD("encode begin. buf size = %zu", buf.size());

    int encodeLen = base64.encode(out, buf.const_data(), buf.size());
    LOGD("encode end. [%d] \n{\n%s\n}", encodeLen, out);

    int decodeLen = base64.decode(from, out, encodeLen);
    LOGD("strlen(from) = %zu, decodeLen = %d", strlen((char *)from), decodeLen);
    LOGD("decode end. decodesize = %d,\n%s", decodeLen, from);
    LOG_ASSERT(decodeLen == buf.size(), "test base64 fialed");
    return 0;
}
