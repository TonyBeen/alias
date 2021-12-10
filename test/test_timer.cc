#include <utils/timer.h>
#include <log/log.h>

#define LOG_TAG "test_timer"

using namespace Jarvis;

TimerManager gTimerManager;

int func(void *arg)
{
    LOGI("func()");
    return 0;
}

int func2(void *arg)
{
    LOGI("func2()");
    return 0;
}

int main()
{
    // 单位ms
    gTimerManager.addTimer(2000, func2, nullptr, 1000);
    uint64_t uniqueId = gTimerManager.addTimer(6000, func, nullptr, 2000);
    LOGI("timer start");
    gTimerManager.StartTimer(true);

    printf(".....\n");
    return 0;
}
