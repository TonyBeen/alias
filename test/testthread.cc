/*************************************************************************
    > File Name: testthread.cpp
    > Author: hsz
    > Desc: 
    > Created Time: 2021年06月19日 星期六 17时53分21秒
 ************************************************************************/

#include <stdio.h>
#include <utils/thread.h>
#include <utils/string8.h>

using namespace eular;

struct Data {
    int num;
    eular::String8 str;
};

class ThreadExt : public ThreadBase {
public:
    ThreadExt() : ThreadBase("test-drived-thread") {}

protected:
    virtual int threadWorkFunction(void *arg) override
    {
        printf("ThreadExt::threadWorkFunction()\n");
    }
};

int function(void *arg)
{
    printf("--- %s: %s()\n", __FILE__, __func__);
    if (arg) {
        Data *data = static_cast<Data *>(arg);
        printf("data[%d, %s]\n", data->num, data->str.c_str());
    }
    int ms = 10;
    printf("--- sleep %d ms tid = %ld\n", ms, gettid());
    msleep(ms);
    return -1;
}

int main(int argc, char **argv)
{
    
    Data *data = new Data;
    if (data == nullptr) {
        perror("malloc");
        return -1;
    }
    data->num = 100;
    data->str = "Hello world!";

    eular::Thread thread(std::bind(function, data), "test-thread");
    thread.join();

    ThreadExt ext;
    ext.run();

    printf("ext tid %d\n", ext.getKernalTid());
    printf("thread name: %s\n", ext.threadName().c_str());
    sleep(1);
    ext.stop();
    printf("\033[32mWhen you see the assertion failure, it means success\033[0m\n");
    assert(false);
    return 0;
}
