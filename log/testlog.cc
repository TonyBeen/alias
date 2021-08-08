#include "log.h"
#include <pthread.h>
#include <unistd.h>

#define LOG_TAG "Test"

void *thread(void *)
{
    int num = 0;
    while (1) {
        LOGI("thread-%.5d", num++);
        usleep(50000);
    }
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, thread, nullptr);
    int num = 0;
    LOG_ASSERT(num == 0, "Should not output");
    while (1) {
        LOGI("main-%d", num++);
        usleep(50000);
    }

    pthread_join(tid, nullptr);
    return 0;
}