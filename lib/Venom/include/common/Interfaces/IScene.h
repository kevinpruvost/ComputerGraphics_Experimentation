#pragma once

#include <common/Error.h>

class IScene
{
public:
    virtual ~IScene() = default;

    virtual Venom::ErrorCode Update() = 0;

    static bool IsStarted();

protected:
    virtual Venom::ErrorCode _Init() = 0;

    static bool _started;
};