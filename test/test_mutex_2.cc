/*************************************************************************
    > File Name: test_mutex_2.cc
    > Author: hsz
    > Brief:
    > Created Time: Wed 23 Nov 2022 10:49:35 AM CST
 ************************************************************************/

/**
 * https://blog.csdn.net/weixin_32147807/article/details/116701847
 * 测试当某一线程在未解锁情况下异常退出时处理
 */

#include <utils/thread.h>
#include <utils/mutex.h>
#include <pthread.h>

eular::Mutex gMutex;
int count = 0;

void thread_1()
{
    while (1) {
        gMutex.lock();
        ++count;
        printf("tid = %ld, count = %d\n", gettid(), count);
        gMutex.unlock();
        msleep(500);
    } 
}

void thread_2()
{
    gMutex.lock();
    printf("%s() thread exit without unlocking\n", __func__);
}

void thread_3()
{
    while (1) {
        gMutex.lock();
        if (count > 0) {
            --count;
        }
        printf("tid = %ld, count = %d\n", gettid(), count);
        gMutex.unlock();
        msleep(400);
    } 
}

int main(int argc, char **argv)
{
    eular::Thread th1(std::bind(thread_1));
    eular::Thread th2(std::bind(thread_2));
    th2.join();
    sleep(2);
    eular::Thread th3(std::bind(thread_3));
    th1.join();
    th3.join();

    return 0;
}