#include <common/FrameworkLoader.h>
#include <common/Exception.h>
#include <common/Rendering.h>
#include <common/ObjectPool.h>
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
        ObjectPool::Clear();
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

    RELOAD_DLL_SINGLETON(Rendering, EngineDll.get());

    __frameworkType = type;
}

BaseFramework* FrameworkLoader::GetFramework() const
{
    return __framework;
}
