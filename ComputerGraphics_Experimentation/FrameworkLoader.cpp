#include "FrameworkLoader.h"
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
    const wchar_t * dllName = nullptr;

    if (hDllInstance) {
        delete m_framework;
        FreeLibrary(hDllInstance);
        m_framework = nullptr;
        hDllInstance = nullptr;
    }

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

    if ((hDllInstance = LoadLibraryW(dllName)) != nullptr) {
        // Get function pointer for creating object from DLL
        void* zboui = GetProcAddress(hDllInstance, "createFrameworkInstance");
        CreateBaseFrameworkFn createFramework = reinterpret_cast<CreateBaseFrameworkFn>(GetProcAddress(hDllInstance, "createFrameworkInstance"));

        if (createFramework != nullptr) {
            // Call function to create object
            m_framework = createFramework();
            if (m_framework == nullptr)
            {
                throw std::runtime_error("Failed to create framework object");
            }
        }
        else {
            // Function not found
            throw std::runtime_error("Failed to find function in DLL");
        }
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
