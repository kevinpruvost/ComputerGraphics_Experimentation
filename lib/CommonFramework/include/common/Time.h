#pragma once

#include <chrono>

class Time
{
public:
    using Clock = std::chrono::steady_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    using Duration = std::chrono::duration<double>;

    static void SetStartTime();
    static void SetNewLambda();
    static double GetLambda();

private:
    static TimePoint __startTime;
    static double __lambda;
};