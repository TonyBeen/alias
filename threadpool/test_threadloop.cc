/*************************************************************************
    > File Name: test_threadloop.cc
    > Author: hsz
    > Brief:
    > Created Time: Thu 28 Oct 2021 03:27:41 PM CST
 ************************************************************************/

#include "threadpool.h"
#include <utils/thread.h>
#include <utils/exception.h>
#include <log/log.h>
#include <iostream>
using namespace std;

#define LOG_TAG "test threadpool"

void func(void *arg)
{
    for (int i = 0; i < 50; ++i) {
        msleep(4);
    }
    LOGI("thread %ld excute over.\n", gettid());
}

int addWorkThread(void *arg)
{
    Jarvis::ThreadPool *threadPool = (Jarvis::ThreadPool *)arg;
    while (1) {
        threadPool->addWork(std::bind(&func, nullptr));
        msleep(80);
    }
}

int main(int argc, char **argv)
{
    Jarvis::Thread thread("addWorkThread", addWorkThread);
    try {
        Jarvis::ThreadPool threadPool(2, 4);
        thread.setArg(&threadPool);
        thread.run();
        for (int i = 0; i < 100; ++i) {
            threadPool.addWork(std::bind(&func, nullptr));
        }
        threadPool.startWork();
        while(1) {
            sleep(1);
        }
    } catch (const Jarvis::Exception &e) {
        cout << e.what() << endl;
    }

    return 0;
}
