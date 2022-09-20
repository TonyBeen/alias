#include "log.h"
#include <pthread.h>
#include <unistd.h>

#define LOG_TAG "Test"

void *thread(void *)
{
    int num = 0;
    while (num < 10) {
        LOGI("thread-%.5d", num++);
        usleep(500000);
    }
}

int main()
{
    eular::InitLog(eular::LogLevel::DEBUG);
    eular::SetPath("/home/eular/log/");
    eular::EnableLogColor(true);
    eular::addOutputNode(eular::LogWrite::FILEOUT);

    LOGD("**************");
    LOGI("**************");
    LOGW("**************");
    LOGE("**************");
    LOGF("**************");

    pthread_t tid;
    pthread_create(&tid, nullptr, thread, nullptr);
    int num = 0;
    LOG_ASSERT(num == 0, "Should not output");
    while (num < 10) {
        LOGI("main-%d", num++);
        usleep(500000);
    }

    pthread_join(tid, nullptr);
    return 0;
}