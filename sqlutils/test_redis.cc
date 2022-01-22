/*************************************************************************
    > File Name: test_redis.cc
    > Author: hsz
    > Brief:
    > Created Time: Mon 10 Jan 2022 04:59:13 PM CST
 ************************************************************************/

#include "redis.h"
#include <assert.h>
#include <iostream>

using namespace std;
using namespace eular;

RedisInterface gRedis;

int main(int argc, char **argv)
{
    assert(gRedis.connect("127.0.0.1", 6379, "eular123") == 0);
    assert(gRedis.setKeyValue("name", "eular") == 0);
    assert(gRedis.isKeyExist("name"));
    assert(gRedis.setKeyLifeCycle("name", 500000000));
    cout << "name.TTL = " << gRedis.getKeyTTLMS("name") << endl;
    std::vector<String8> val;
    std::vector<String8> key = {"name"};
    cout << "name.value = " << gRedis.getKeyValue("name") << endl;
    gRedis.delKey("name");
    assert(gRedis.isKeyExist("name") == false);

    
    gRedis.disconnect();
    return 0;
}
