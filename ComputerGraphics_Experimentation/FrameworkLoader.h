#pragma once
#include <common/BaseFramework.h>
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
    ~FrameworkLoader() = default;

    void LoadFramework(const FrameworkType type);
    BaseFramework * GetFramework() const;

private:
    FrameworkType m_frameworkType;
    HINSTANCE hDllInstance = nullptr;
    BaseFramework* m_framework;
};