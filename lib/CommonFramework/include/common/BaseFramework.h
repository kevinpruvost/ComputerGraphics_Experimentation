#pragma once

#include <common/Logger.h>
#include <common/Config.h>
#include <common/Exception.h>
#include <common/ApplicationCommonBase.h>

class COMMONFRAMEWORK_API BaseFramework {
public:
    BaseFramework();
    virtual ~BaseFramework();

    // Initialization function
    virtual bool Init() = 0;

    // Destruction function
    virtual void Destroy() = 0;

    // Launch function
    virtual int Launch() = 0;

    // Get the logger
    virtual Logger * GetLogger() = 0;

    // Get the config
    virtual Config * GetConfig() = 0;

    void SetConfig(const Config & config);
    void SetWindow(Window* w);

protected:
    ApplicationCommonBase * _app;

private:
};