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
        usleep(1000 * 100);
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
        usleep(1000 * 200);
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
    return Thread::THREAD_EXIT;
}

void unnamed_thread_main()
{
    eular::Thread th("test sem", unnamed_thread_func);
    th.run();
    testUnnamedSemaphore();
}

eular::Sem gSemNamed(nullptr, 0);

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
    return Thread::THREAD_EXIT;
}

void named_thread_main()
{
    eular::Thread th("test sem", named_thread_func);
    th.run();
    test_named_sem();
}

int main()
{
    named_thread_main();
    return 0;
}