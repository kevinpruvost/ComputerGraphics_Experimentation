#include <common/Engine/EngineLoader.h>
#include <common/Exception.h>
#include <common/Rendering.h>
#include <iostream>
#include <cstdlib>

#define LOAD_SINGLETON(Type) Type::ReloadInstance(EngineDll.get(), _STRINGIZE(DLL_SINGLETON_LOADING_NAME(Type)))

UPtr<DLL> EngineLoader::EngineDll(nullptr);

EngineLoader::EngineLoader(const EngineAPI type)
    : __framework(nullptr)
{
    LoadEngine(type);
}

EngineLoader::~EngineLoader()
{
}

typedef BaseFramework* (*CreateBaseFrameworkFn)();

void EngineLoader::LoadEngine(const EngineAPI type)
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
    
    LOAD_SINGLETON(Rendering);

    __frameworkType = type;
}

BaseFramework* EngineLoader::GetFramework() const
{
    return __framework;
}

DLL* EngineLoader::GetEngineDll()
{
    return EngineDll.get();
}
