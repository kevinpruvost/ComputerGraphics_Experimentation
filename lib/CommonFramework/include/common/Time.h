#pragma once

#include <chrono>

class Time
{
public:
    using Clock = std::chrono::steady_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    using Duration = std::chrono::microseconds;

    static void SetStartTime();
    static void SetNewLambda();
    static double GetLambda();
    static double GetLambdaFromBeginning();

private:
    static TimePoint __startTime;
    static double __lambda, __lambdaFromBeginning;
};