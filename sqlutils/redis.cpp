/*************************************************************************
    > File Name: redis.cpp
    > Author: hsz
    > Brief:
    > Created Time: Mon 10 Jan 2022 03:12:48 PM CST
 ************************************************************************/

// #define _DEBUG
#include "redis.h"
#include <utils/debug.h>
#include <utils/Errors.h>
#include <log/log.h>

#define LOG_TAG "redis"

#define REDIS_STATUS_OK 0                   // 正常
#define REDIS_STATUS_CONNECT_ERROR      -1  // 连接失败
#define REDIS_STATUS_AUTH_ERROR         -2  // 鉴权失败
#define REDIS_STATUS_QUERY_ERROR        -3  // 查询失败
#define REDIS_STATUS_NOT_CONNECTED      -4  // 未连接
#define REDIS_STATUS_UNAUTHENTICATED    -5  // 未鉴权

namespace eular {
RedisInterface::RedisInterface() :
    mRedisCtx(nullptr)
{

}

RedisInterface::RedisInterface(const String8 &ip, uint16_t port, const char *pwd) :
    mRedisCtx(nullptr)
{
    connect(ip, port, pwd);
}

RedisInterface::~RedisInterface()
{
    disconnect();
}

/**
 * @brief 连接redis数据库0
 * 
 * @param ip redis服务地址
 * @param port redis服务端口
 * @param pwd redis密码
 * @return int 成功返回0，连接失败/鉴权失败返回负值
 */
int RedisInterface::connect(const String8 &ip, uint16_t port, const char *pwd)
{
    if (mRedisCtx != nullptr) {
        return REDIS_STATUS_OK;
    }

    mRedisHost = ip;
    mRedisPort = port;
    mRedisPwd = pwd;

    mRedisCtx = redisConnect(mRedisHost.c_str(), mRedisPort);
    if (mRedisCtx == nullptr) {
        LOG("%s() redisConnect (%s, %u) error.", __func__, mRedisHost.c_str(), mRedisPort);
        return REDIS_STATUS_CONNECT_ERROR;
    }

    if (authenticate(pwd)) {
        return REDIS_STATUS_OK;
    }

    redisFree(mRedisCtx);
    mRedisCtx = nullptr;
    return REDIS_STATUS_AUTH_ERROR;
}

bool RedisInterface::disconnect()
{
    if (mRedisCtx) {
        redisFree(mRedisCtx);
        mRedisCtx = nullptr;
    }

    return true;
}

bool RedisInterface::authenticate(const char *pwd)
{
    if (pwd == nullptr) {
        return false;
    }
    redisReply *reply = nullptr;
    if (mRedisCtx) {
        static char buf[32];
        memset(buf, 0, sizeof(buf));
        snprintf(buf, sizeof(buf), "auth %s", pwd);
        reply = (redisReply *)redisCommand(mRedisCtx, buf);
        if (reply == nullptr || reply->type == REDIS_REPLY_ERROR) {
            goto end;
        }

        freeReplyObject(reply);
        return true;
    }

end:
    int len = strlen(pwd);
    char tmpBuf[32] = {0};
    len = len > 31 ? 31 : len;
    memcpy(tmpBuf, pwd, len);
    if (len <= 4) {
        memset(tmpBuf, '*', len);
    } else {
        int begin = len / 2 - 2;
        memset(tmpBuf + begin, '*', 4);
    }

    if (reply) {
        freeReplyObject(reply);
        reply = nullptr;
    }
    LOG("%s() auth %s error.\n", __func__, tmpBuf);
    return false;
}


int RedisInterface::SqlCommand(const String8 &sql)
{

}


const char *RedisInterface::strerror(int no) const
{
    const char *ret = nullptr;
    static const char *msgArray[] = {
        "OK",
        "Connect Error",
        "Authentication Failed",
        "Query Error",
        "Not Connected",
        "Unauthenticated"
    };

    static const uint16_t arraySize = sizeof(msgArray) / sizeof(char *);
    
    int index = -no;
    if (index >= 0 && index < arraySize) {
        ret = msgArray[index];
    } else {
        ret = "Unknow Error";
    }

    return ret;
}

}