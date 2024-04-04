#include <common/FrameworkLoader.h>
#include <common/Exception.h>
#include <iostream>
#include <cstdlib>

UPtr<DLL> FrameworkLoader::EngineDll(nullptr);

FrameworkLoader::FrameworkLoader(const EngineAPI type)
    : __framework(nullptr)
{
    LoadFramework(type);
}

FrameworkLoader::~FrameworkLoader()
{
}

typedef BaseFramework* (*CreateBaseFrameworkFn)();

void FrameworkLoader::LoadFramework(const EngineAPI type)
{
    const wchar_t * dllName = nullptr;

    if (EngineDll) {
        __framework = nullptr;
    }

    switch (type)
    {
    case EngineAPI::OpenGL:
        dllName = L"OpenGL_DLL.dll";
        break;
    case EngineAPI::DirectX11:
        dllName = L"DX11_DLL.dll";
        break;
    case EngineAPI::Vulkan:
        dllName = L"Vulkan_DLL.dll";
        break;
    }

    // Load the DLL
    EngineDll.reset(new DLL(dllName));
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
