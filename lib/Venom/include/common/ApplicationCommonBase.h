#pragma once

#include <common/Common.h>
#include <common/Window.h>
#include <common/DLL.h>

#include <memory>

class VENOM_API ApplicationCommonBase
{
public:
    void SetConfig(const Config & config);
    virtual Venom::ErrorCode Update() = 0;
    ~ApplicationCommonBase();

protected:
    ApplicationCommonBase();
    EngineSettings _engineSettings;
};

