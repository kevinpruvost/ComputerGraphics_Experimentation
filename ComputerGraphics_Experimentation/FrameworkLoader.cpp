#include "FrameworkLoader.h"
#include <common/Exception.h>
#include <iostream>
#include <cstdlib>

FrameworkLoader::FrameworkLoader(const FrameworkType type)
    : __framework(nullptr), __dll(nullptr)
{
    LoadFramework(type);
}

FrameworkLoader::~FrameworkLoader()
{
    if (__dll) {
        delete __dll;
    }
}

typedef BaseFramework* (*CreateBaseFrameworkFn)();

void FrameworkLoader::LoadFramework(const FrameworkType type)
{
    const wchar_t * dllName = nullptr;

    if (__dll) {
        delete __dll;
        __framework = nullptr;
        __dll = nullptr;
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

    // Load the DLL
    __dll = new DLL(dllName);
    CreateBaseFrameworkFn createFramework = reinterpret_cast<CreateBaseFrameworkFn>(__dll->getFunction("createFrameworkInstance"));
    if (createFramework != nullptr) {
        // Call function to create object
        __framework = createFramework();
        if (__framework == nullptr)
        {
            throw DLLException("Failed to create framework object");
        }
    }
    else {
        // Function not found
        throw DLLException("Failed to find function in DLL: {}", dllName);
    }

    __frameworkType = type;
}

BaseFramework* FrameworkLoader::GetFramework() const
{
    return __framework;
}
