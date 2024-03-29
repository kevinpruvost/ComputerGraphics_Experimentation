#include <common/FrameworkLoader.h>
#include <common/Exception.h>
#include <iostream>
#include <cstdlib>

DLL* FrameworkLoader::EngineDll = nullptr;

FrameworkLoader::FrameworkLoader(const FrameworkType type)
    : __framework(nullptr)
{
    LoadFramework(type);
}

FrameworkLoader::~FrameworkLoader()
{
    if (EngineDll) {
        delete EngineDll;
    }
}

typedef BaseFramework* (*CreateBaseFrameworkFn)();

void FrameworkLoader::LoadFramework(const FrameworkType type)
{
    const wchar_t * dllName = nullptr;

    if (EngineDll) {
        delete EngineDll;
        __framework = nullptr;
        EngineDll = nullptr;
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
    EngineDll = new DLL(dllName);
    CreateBaseFrameworkFn createFramework = EngineDll->getFunction<CreateBaseFrameworkFn>("createFrameworkInstance");
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
        throw DLLException("Failed to find function in DLL: {}", (char *)dllName);
    }

    __frameworkType = type;
}

BaseFramework* FrameworkLoader::GetFramework() const
{
    return __framework;
}
