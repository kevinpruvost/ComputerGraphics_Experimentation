#include <common/Time.h>

Time::TimePoint Time::__startTime;
double Time::__lambda = 0.0f;

void Time::SetStartTime()
{
    __startTime = Clock::now();
}

void Time::SetNewLambda()
{
    __lambda = std::chrono::duration_cast<Duration>(Clock::now() - __startTime).count();
    __startTime = Clock::now();
}

double Time::GetLambda()
{
    return __lambda;
}
