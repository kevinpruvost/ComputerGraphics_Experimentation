#pragma once

#include <common/Logger.h>
#include <common/Config.h>

#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)

class BaseFramework {
public:
    BaseFramework();
    virtual ~BaseFramework() = default;

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
private:
};