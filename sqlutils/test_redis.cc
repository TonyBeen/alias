/*************************************************************************
    > File Name: test_redis.cc
    > Author: hsz
    > Brief:
    > Created Time: Mon 10 Jan 2022 04:59:13 PM CST
 ************************************************************************/

#include "redis.h"
#include <iostream>

using namespace std;
using namespace eular;

int main(int argc, char **argv)
{
    RedisInterface redis;
    int ret = 0;
    if ((ret = redis.connect("127.0.0.1", 6379, "eular123")) < 0) {
        printf("%d, %s\n", ret, redis.strerror(ret));
        return 0;
    }

    
    return 0;
}
