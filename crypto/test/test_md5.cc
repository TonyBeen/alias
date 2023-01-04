/*************************************************************************
    > File Name: test_md5.cc
    > Author: eular
    > Brief: 测试md5摘要
    > Created Time: Tue 28 Dec 2021 09:50:21 AM CST
 ************************************************************************/

#include "md5.h"
#include <utils/buffer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <log/log.h>

using namespace eular;
using namespace std;

#define LOG_TAG "Test Md5"

int main(int argc, char **argv)
{
    LOGD("main begin");
    Md5 md5;
    ByteBuffer buf;
    uint8_t out[Md5::MD5_BUF_SIZE] = {0};
    const uint8_t *from = buf.const_data();
    uint32_t fromLen = 0;

    if (argc != 2) {
        printf("usage: %s file\n", argv[0]);
        return -1;
    }

    LOGD("read %s begin", argv[1]);
    FILE *fp = fopen(argv[1], "r");
    if (fp == nullptr) {
        perror("fopen error");
        return -1;
    }
    int readSize = 0;
    uint8_t tmp[256] = {0};
    while (!feof(fp)) {
        readSize = fread(tmp, 1, 256, fp);
        if (readSize > 0) {
            buf.append(tmp, readSize);
        } else if (readSize < 0) {
            perror("fread error");
            return -1;
        }
    }
    LOGD("read end");
    struct stat st;
    stat(argv[1], &st);
    char timeBuf[128] = {0};
    strftime(timeBuf, 128, "%Y-%m-%d %H:%M:%S", localtime(&st.st_ctim.tv_sec));
    LOGD("file size: %zuByte; last status change time: %s", st.st_size, timeBuf);
    from = buf.const_data();
    fromLen = buf.size();
    LOGD("encode begin");
    md5.encode(out, from, fromLen);
    LOGD("encode end");

    printf("after encode:\n");
    for (int i = 0; i < Md5::MD5_BUF_SIZE; ++i) {
        if (i != 0 && i % 16 == 0) {
            printf("\n");
        }
        printf("%02x ", out[i]);
    }
    printf("\n");
    LOGD("main end");
    return 0;
}
