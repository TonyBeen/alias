#include <utils/timer.h>
#include <log/log.h>

#define LOG_TAG "test_timer"

using namespace eular;

TimerManager gTimerManager;

int func(void *arg)
{
    LOGI("func()");
    return 0;
}

int func2(void *arg)
{
    int *num = static_cast<int *>(arg);
    ++*num;
    LOGI("func2() %p, %d", num, *num);
    return 0;
}

void test_main_loop()
{
    std::shared_ptr<void> ptr((new int(10)));
    gTimerManager.addTimer(2000, func2, ptr, 1000);
    uint64_t uniqueId = gTimerManager.addTimer(6000, func, nullptr, 2000);
    LOGI("timer start");
    gTimerManager.StartTimer(true);
}

void test_thread_loop()
{
    LOGI("timer start");
    gTimerManager.StartTimer(false);
    gTimerManager.addTimer(2000, func2, nullptr, 1000);
    uint64_t uniqueId = gTimerManager.addTimer(6000, func, nullptr, 2000);

    while (1) {
        /* code */
    }
}

int main()
{
    test_main_loop();
    printf(".....\n");
    return 0;
}
