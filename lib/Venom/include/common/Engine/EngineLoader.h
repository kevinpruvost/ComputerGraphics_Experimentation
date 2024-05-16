#pragma once
#include "EngineObject.h"
#include <common/EngineSettings.h>
#include <common/BaseFramework.h>
#include <common/DLL.h>
#include <windows.h>

class VENOM_API EngineLoader
{
public:
    EngineLoader(const GraphicsEngineAPI type = GraphicsEngineAPI::Vulkan);
    ~EngineLoader();

    void LoadEngine(const GraphicsEngineAPI type);
    BaseFramework * GetEngine() const;
public:
    static DLL * GetEngineDll();
private:
    static UPtr<DLL> EngineDll;

private:
    GraphicsEngineAPI __frameworkType;
    BaseFramework* __framework;
};