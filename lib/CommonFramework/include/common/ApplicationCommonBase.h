#pragma once

#include <common/Common.h>
#include <common/Window.h>
#include <common/DLL.h>

#include <memory>

class ApplicationCommonBase
{
public:
    COMMONFRAMEWORK_API void SetWindow(Window * w);
    COMMONFRAMEWORK_API void SetConfig(const Config & config);
    virtual Venom::ErrorCode Run() = 0;
    COMMONFRAMEWORK_API ~ApplicationCommonBase();

protected:
    COMMONFRAMEWORK_API ApplicationCommonBase();
    Window * _w;
    EngineSettings _engineSettings;
};

