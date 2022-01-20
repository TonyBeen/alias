/*************************************************************************
    > File Name: redis.h
    > Author: hsz
    > Brief:
    > Created Time: Mon 10 Jan 2022 03:12:44 PM CST
 ************************************************************************/

#ifndef __REDIS_H__
#define __REDIS_H__

#include <utils/string8.h>
#include <utils/exception.h>
#include <hiredis/hiredis.h>

namespace eular {

class RedisInterface {
public:
    RedisInterface();
    RedisInterface(const String8 &ip, uint16_t port, const char *pwd);
    virtual ~RedisInterface();

    int  connect(const String8 &ip, uint16_t port = 6379, const char *pwd = nullptr);
    bool disconnect();
    bool authenticate(const char *pwd);

    int SqlCommand(const String8 &sql);

    const char *strerror(int no) const;
    void setHost(const String8 &ip) { mRedisHost = ip; }
    void setPort(const uint16_t &port) { mRedisPort = port; }
    void setPassword(const String8 &pwd) { mRedisPwd = pwd; }
    redisReply *getRedisReply() { return mRedisReply; }

protected:
    redisContext   *mRedisCtx;
    redisReply     *mRedisReply;

    String8         mRedisHost;
    uint16_t        mRedisPort;
    String8         mRedisPwd;
};

}

#endif // __REDIS_H__
