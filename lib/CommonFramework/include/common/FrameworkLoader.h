#pragma once
#include <common/EngineSettings.h>
#include <common/BaseFramework.h>
#include <common/DLL.h>
#include <windows.h>

class COMMONFRAMEWORK_API FrameworkLoader
{
public:
    FrameworkLoader(const EngineAPI type = EngineAPI::Vulkan);
    ~FrameworkLoader();

    void LoadFramework(const EngineAPI type);
    BaseFramework * GetFramework() const;
public:
    static UPtr<DLL> EngineDll;

private:
    EngineAPI __frameworkType;
    BaseFramework* __framework;
};