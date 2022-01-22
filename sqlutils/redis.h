/*************************************************************************
    > File Name: redis.h
    > Author: hsz
    > Brief:
    > Created Time: Mon 10 Jan 2022 03:12:44 PM CST
 ************************************************************************/

#ifndef __REDIS_H__
#define __REDIS_H__

#include <utils/utils.h>
#include <utils/string8.h>
#include <utils/exception.h>
#include <hiredis/hiredis.h>
#include <memory>
#include <vector>

namespace eular {

class RedisReply {
public:
    RedisReply(const RedisReply&) = delete;
    RedisReply &operator=(const RedisReply&) = delete;
    virtual ~RedisReply();

    String8 getString();

private:
    RedisReply(redisReply *reply);

    std::shared_ptr<redisReply> mReply;
    bool isVaild;
    friend class RedisInterface;
};

class RedisInterface {
public:
    RedisInterface();
    RedisInterface(const String8 &ip, uint16_t port, const char *pwd);
    DISALLOW_COPY_AND_ASSIGN(RedisInterface);
    virtual ~RedisInterface();

    int  connect(const String8 &ip, uint16_t port = 6379, const char *pwd = nullptr);
    int  connecttimeout(const String8 &ip, uint16_t port, const char *pwd, const struct timeval &timeout);
    bool disconnect();
    bool authenticate(const char *pwd);

    RedisReply *SqlCommand(const String8 &sql);

    int selectDB(uint16_t dbNum);
    std::vector<String8> showKeys(const String8 &pattern); // not implemented
    int  setKeyValue(const String8 &key, const String8 &val);
    String8  getKeyValue(const String8 &key);
    int  getKeyValue(const std::vector<String8> &keyVec, std::vector<String8> &valVec);
    bool isKeyExist(const String8 &key);
    bool delKey(const String8 &key);
    bool setKeyLifeCycle(const String8 &key, uint64_t milliseconds, bool isTimeStamp = false);
    bool delKeyLifeCycle(const String8 &key);
    int64_t getKeyTTLMS(const String8 &key);

    // hash
    int hashCreateOrReplace(const String8 &key,
        const std::vector<std::pair<String8, String8>> &filedValue);
    int hashTableSetFiledValue(const String8 &key,
        const String8 &filed, const String8 &value);
    int hashGetKeyFiled(const String8 &key, const String8 &filed, String8 &ret);
    int hashGetAllKey(const String8 &key, std::vector<std::pair<String8, String8>> &ret);
    int hashDelKeyOrFiled(const String8 &key, const char **filed, uint32_t fileds);
    int hashDelKeyOrFiled(const String8 &key, const std::vector<String8> &filedVec);

    // list


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
