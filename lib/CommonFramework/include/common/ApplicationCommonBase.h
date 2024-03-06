#pragma once

#include <common/Common.h>
#include <common/Window.h>

#include <memory>

class ApplicationCommonBase
{
public:
    void SetWindow(Window * w);
    void SetConfig(const Config & config);
    virtual ErrorCode Run() = 0;
    ~ApplicationCommonBase();

protected:
    ApplicationCommonBase();
    Window * _w;
    EngineSettings _engineSettings;
};

