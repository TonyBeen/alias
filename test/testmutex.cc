#include <utils/mutex.h>
#include <utils/string8.h>
#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <memory>

using namespace std;
using namespace Jarvis;

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

int main()
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
    return 0;
}