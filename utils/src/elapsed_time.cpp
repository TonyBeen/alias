/*************************************************************************
    > File Name: elapsed_time.cpp
    > Author: hsz
    > Brief:
    > Created Time: Sun 25 Feb 2024 03:14:02 PM CST
 ************************************************************************/

#include "utils/elapsed_time.h"
#include <assert.h>
#include "utils/utils.h"

namespace eular {
ElapsedTime::ElapsedTime() :
    mTimeType(ElapsedTimeType::MILLISECOND),
    mBeginTime(0)
{
}

ElapsedTime::ElapsedTime(ElapsedTimeType type) :
    mTimeType(type),
    mBeginTime(0)
{
}

ElapsedTime::~ElapsedTime()
{
}

void ElapsedTime::start()
{
    if (0 == mBeginTime)
    {
        mBeginTime = getCurrentTime();
    }
}

void ElapsedTime::stop()
{
    if (0 == mBeginTime)
    {
        return;
    }

    uint64_t endTime = getCurrentTime();
    assert(endTime >= mBeginTime);
    if (eular_likely(endTime >= mBeginTime))
    {
        mElapsedTimeList.push_back(std::make_pair(mBeginTime, endTime));
    }

    mBeginTime = 0;
}

uint64_t ElapsedTime::elapsedTime()
{
    uint64_t nElapsedTime = 0;

    for (const auto &pairIt : mElapsedTimeList)
    {
        nElapsedTime += (pairIt.second - pairIt.first);
    }

    mElapsedTimeList.clear();
    return nElapsedTime;
}

void ElapsedTime::reset()
{
    mElapsedTimeList.clear();
    mBeginTime = 0;
}

uint64_t ElapsedTime::getCurrentTime() const
{
    int64_t currentTime = 0;
    std::chrono::steady_clock::time_point tm = std::chrono::steady_clock::now();
    switch (mTimeType) {
    case ElapsedTimeType::SECOND:
        currentTime = std::chrono::duration_cast<std::chrono::seconds>(tm.time_since_epoch()).count();
        break;
    case ElapsedTimeType::MILLISECOND:
        currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(tm.time_since_epoch()).count();
        break;
    case ElapsedTimeType::MICROSECOND:
        currentTime = std::chrono::duration_cast<std::chrono::microseconds>(tm.time_since_epoch()).count();
        break;
    case ElapsedTimeType::NANOSECOND:
        currentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(tm.time_since_epoch()).count();
        break;
    default:
        break;
    }

    return static_cast<uint64_t>(currentTime);
}

} // namespace eular
