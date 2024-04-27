#include <common/Time.h>
#include <common/Logger.h>

Time::TimePoint Time::__startTime;
double Time::__lambda = 0.0f;
double Time::__lambdaFromBeginning = 0.0f;

long long int frames = 0;
Time::TimePoint fpsTime;

void Time::SetStartTime()
{
    __startTime = Clock::now();
    fpsTime = Clock::now();
}

void Time::SetNewLambda()
{
    long long microseconds = std::chrono::duration_cast<Duration>(Clock::now() - __startTime).count();
    __lambda = microseconds / 1000000.0f;
    __lambdaFromBeginning += __lambda;
    __startTime = Clock::now();
    ++frames;

    if (std::chrono::duration_cast<Duration>(Clock::now() - fpsTime).count() >= 1000000)
    {
        Logger::DebugPrint("FPS: %d", frames);
        frames = 0;
        fpsTime = Clock::now();
    }
}

double Time::GetLambda()
{
    return __lambda;
}

double Time::GetLambdaFromBeginning()
{
    return __lambdaFromBeginning;
}
