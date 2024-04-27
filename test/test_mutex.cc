#include <utils/mutex.h>
#include <utils/string8.h>
#include <utils/thread.h>
#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <memory>

using namespace std;
using namespace eular;

Mutex gMutex;

void *func(void *arg)
{
    shared_ptr<String8> *ptr = static_cast<shared_ptr<String8> *>(arg);
    while(1) {
        {
            AutoLock<Mutex> _l(gMutex);
            printf("func before [%s]\n", (*ptr)->c_str());
            (*ptr)->clear();
            (*ptr)->append("12345");
            printf("func after [%s]\n\n", (*ptr)->c_str());
        }
        usleep(1000 * 500);
    }
}

void testmutex()
{
    shared_ptr<String8> ptr(new String8(""));
    void *tmp = static_cast<void *>(&ptr);
    pthread_t pid;
    int ret = pthread_create(&pid, nullptr, func, tmp);
    assert(ret == 0);
    while(1) {
        {
            AutoLock<Mutex> _l(gMutex);
            printf("before [%s]\n", ptr->c_str());
            ptr->clear();
            ptr->append("abcde");
            printf("after [%s]\n\n", ptr->c_str());
        }
        usleep(1000 * 600);
    }

    pthread_join(pid, nullptr);
}

eular::Sem gSemaphore(0);

void testUnnamedSemaphore()
{
    gSemaphore.wait();
    printf("%s() got singal\n", __func__);
}

int unnamed_thread_func(void *)
{
    printf("%s()\nafter a second will call post()\n", __func__);
    sleep(1);
    gSemaphore.post();
    return 0;
}

void unnamed_thread_main()
{
    eular::Thread th(std::bind(unnamed_thread_func, nullptr), "test sem");
    th.detach();
    testUnnamedSemaphore();
}

eular::Sem gSemNamed("sem", 0);

void test_named_sem()
{
    gSemNamed.wait();
    printf("%s() got singal\n", __func__);
}

int named_thread_func(void *)
{
    printf("%s()\nafter a second will call post()\n", __func__);
    sleep(1);
    gSemNamed.post();
    return 0;
}

void named_thread_main()
{
    eular::Thread th(std::bind(named_thread_func, nullptr), "test sem");
    th.detach();
    test_named_sem();
}

RWMutex rwMutex;
static int rwmutex_count = 0;

int rwmutex_thread(void *arg)
{
    while (1) {
        WRAutoLock<RWMutex> wrlock(rwMutex);
        rwmutex_count++;
        printf("[%ld] ++ count = %d\n", gettid(), rwmutex_count);
        if (rwmutex_count == 10) {
            break;
        }
    }
}

void test_rwmutex()
{
    Thread th(std::bind(rwmutex_thread, nullptr), "test rwmutex");
    th.detach();
    while (1) {
        RDAutoLock<RWMutex> rdlock(rwMutex);
        printf("[%ld] count = %d\n", gettid(), rwmutex_count);
        if (rwmutex_count == 10) {
            break;
        }
        msleep(100);
    }
}

int main()
{
    testmutex();
    // named_thread_main();
    // test_rwmutex();
    return 0;
}