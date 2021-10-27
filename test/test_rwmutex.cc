#include <utils/mutex.h>
#include <utils/utils.h>
#include <utils/string8.h>
#include <utils/thread.h>
#include <assert.h>
#include <stdio.h>

using namespace std;
using namespace Jarvis;

static char buf[128] = {0};
RWMutex gRwMutex;

int read_func(void *)
{
    while (1) {
        {
            RDAutoLock<RWMutex> lock(gRwMutex);
            printf("[%ld]buf: %s\n", gettid(), buf);
        }
        usleep(100 * 1000);
    }
}

int write_func(void *)
{
    int num = 0;
    while (1) {
        {
            WRAutoLock<RWMutex> lock(gRwMutex);
            snprintf(buf, 127, "num = %d", ++num);
        }
        usleep(50 * 1000);
    }
}

int main()
{
    Thread t1("read", read_func);
    Thread t2("write", write_func);

    t1.run();
    t2.run();

    while (1) {
        sleep(1);
    }
    return 0;
}