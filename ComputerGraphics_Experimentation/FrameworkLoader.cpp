#include "FrameworkLoader.h"
#include <windows.h>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <cstdlib>

FrameworkLoader::FrameworkLoader(const FrameworkType type)
{
    LoadFramework(type);
}

typedef BaseFramework* (*CreateBaseFrameworkFn)();

void FrameworkLoader::LoadFramework(const FrameworkType type)
{
    HINSTANCE hDllInstance = nullptr;
    BaseFramework* fw = nullptr;

    const wchar_t * dllName = nullptr;
    switch (type)
    {
    case FrameworkType::OpenGL:
        dllName = L"OpenGL_DLL.dll";
        break;
    case FrameworkType::DirectX11:
        dllName = L"DX11_DLL.dll";
        break;
    case FrameworkType::Vulkan:
        dllName = L"Vulkan_DLL.dll";
        break;
    }

    hDllInstance = LoadLibrary(dllName);

    if (hDllInstance != nullptr) {
        // Get function pointer for creating object from DLL
        CreateBaseFrameworkFn createFramework = reinterpret_cast<CreateBaseFrameworkFn>(GetProcAddress(hDllInstance, "Framework"));

        if (createFramework != nullptr) {
            // Call function to create object
            fw = createFramework();
            if (fw == nullptr)
            {
                throw std::runtime_error("Failed to create framework object");
            }
        }

        // Free the DLL module
        FreeLibrary(hDllInstance);
    }
    else {
        // DLL loading failed
        throw std::runtime_error("Failed to load DLL");
    }

    m_frameworkType = type;
}

BaseFramework* FrameworkLoader::GetFramework() const
{
    return m_framework;
}
