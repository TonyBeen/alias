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
#include <utils/mutex.h>
#include <hiredis/hiredis.h>
#include <memory>
#include <vector>

namespace eular {

class RedisReply {
public:
    typedef std::shared_ptr<RedisReply> SP;

    RedisReply(redisReply *reply);
    RedisReply(const RedisReply&) = delete;
    RedisReply &operator=(const RedisReply&) = delete;
    virtual ~RedisReply();

    String8 getString();

private:
    std::shared_ptr<redisReply> mReply;
    bool isVaild;
    friend class RedisInterface;
};

class RedisInterface {
public:
    typedef std::shared_ptr<RedisInterface> SP;

    RedisInterface();
    RedisInterface(const String8 &ip, uint16_t port, const char *pwd);
    DISALLOW_COPY_AND_ASSIGN(RedisInterface);
    virtual ~RedisInterface();

    int  connect(const String8 &ip, uint16_t port = 6379, const char *pwd = nullptr);
    int  connecttimeout(const String8 &ip, uint16_t port, const char *pwd, const struct timeval &timeout);
    bool disconnect();
    bool authenticate(const char *pwd);

    RedisReply::SP command(const String8 &sql);

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
    int hashGetKeyAll(const String8 &key, std::vector<std::pair<String8, String8>> &ret);
    int hashDelKeyOrFiled(const String8 &key, const char **filed, uint32_t fileds);
    int hashDelKeyOrFiled(const String8 &key, const std::vector<String8> &filedVec);

    // list
    int listInsertFront(const String8 &key, const String8 &value);
    int listInsertFront(const String8 &key, const std::vector<String8> &valueVec);
    int listInsertBack(const String8 &key, const String8 &value);
    int listInsertBack(const String8 &key, const std::vector<String8> &valueVec);
    int listDeleteFront(const String8 &key, const String8 &value);
    int listDeleteBack(const String8 &key, const String8 &value);
    int listDeleteAll(const String8 &key, const String8 &value);
    int listPopFront(const String8 &key, String8 &value);
    int listGetAll(const String8 &key, std::vector<String8> &vec);
    ssize_t listLength(const String8 &key);

    const char *strerror(int no) const;
    void setHost(const String8 &ip) { mRedisHost = ip; }
    void setPort(const uint16_t &port) { mRedisPort = port; }
    void setPassword(const String8 &pwd) { mRedisPwd = pwd; }
    RedisReply::SP getRedisReply() { return std::make_shared<RedisReply>(mRedisReply); }

protected:
    redisContext   *mRedisCtx;
    redisReply     *mRedisReply;
    Mutex           mMutex;

    String8         mRedisHost;
    uint16_t        mRedisPort;
    String8         mRedisPwd;
};

}

#endif // __REDIS_H__
