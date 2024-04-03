#pragma once
#include <common/BaseFramework.h>
#include <common/DLL.h>
#include <windows.h>

class FrameworkLoader
{
public:
    enum class FrameworkType : int
    {
        OpenGL,
        DirectX11,
        Vulkan
    };

    COMMONFRAMEWORK_API FrameworkLoader(const FrameworkType type = FrameworkType::Vulkan);
    COMMONFRAMEWORK_API ~FrameworkLoader();

    COMMONFRAMEWORK_API void LoadFramework(const FrameworkType type);
    COMMONFRAMEWORK_API BaseFramework * GetFramework() const;
public:
    COMMONFRAMEWORK_API static DLL * EngineDll;

private:
    FrameworkType __frameworkType;
    BaseFramework* __framework;
};