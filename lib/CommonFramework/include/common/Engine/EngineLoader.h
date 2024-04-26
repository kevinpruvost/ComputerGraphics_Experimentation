#pragma once
#include "EngineObject.h"
#include <common/EngineSettings.h>
#include <common/BaseFramework.h>
#include <common/DLL.h>
#include <windows.h>

class COMMONFRAMEWORK_API EngineLoader
{
public:
    EngineLoader(const EngineAPI type = EngineAPI::Vulkan);
    ~EngineLoader();

    void LoadEngine(const EngineAPI type);
    BaseFramework * GetFramework() const;
public:
    static DLL * GetEngineDll();
private:
    static UPtr<DLL> EngineDll;

private:
    EngineAPI __frameworkType;
    BaseFramework* __framework;
};