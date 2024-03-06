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

    FrameworkLoader(const FrameworkType type = FrameworkType::Vulkan);
    ~FrameworkLoader();

    void LoadFramework(const FrameworkType type);
    BaseFramework * GetFramework() const;

private:
    FrameworkType __frameworkType;
    BaseFramework* __framework;
    DLL * __dll;
};