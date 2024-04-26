#pragma once

#include <common/Common.h>

class EngineObject
{
public:
    EngineObject();
    ~EngineObject();

    void DeleteObject();
    virtual Venom::ErrorCode ReloadObjectFromEngine() = 0;
};