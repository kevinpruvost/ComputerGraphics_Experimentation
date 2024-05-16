#pragma once

#include <common/Logger.h>
#include <common/Config.h>
#include <common/Exception.h>
#include <common/ApplicationCommonBase.h>

class VENOM_API BaseFramework {
public:
    BaseFramework();
    virtual ~BaseFramework();

    // Initialization function
    virtual Venom::ErrorCode Init() = 0;

    // Update function
    virtual Venom::ErrorCode Update() = 0;

    // Destruction function
    virtual void Destroy() = 0;

    // Get the logger
    virtual Logger * GetLogger() = 0;

    // Get the config
    virtual Config * GetConfig() = 0;

    void SetConfig(const Config & config);

protected:
    ApplicationCommonBase * _app;

private:
};