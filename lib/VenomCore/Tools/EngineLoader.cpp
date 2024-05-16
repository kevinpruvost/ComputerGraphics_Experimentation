#include <common/Engine/EngineLoader.h>
#include <common/Exception.h>
#include <common/Rendering.h>
#include <iostream>
#include <cstdlib>

#define LOAD_SINGLETON(Type) Type::ReloadInstance(EngineDll.get(), _STRINGIZE(DLL_SINGLETON_LOADING_NAME(Type)))

UPtr<DLL> EngineLoader::EngineDll(nullptr);

EngineLoader::EngineLoader(const GraphicsEngineAPI type)
    : __framework(nullptr)
{
    LoadEngine(type);
}

EngineLoader::~EngineLoader()
{
}

typedef BaseFramework* (*CreateBaseFrameworkFn)();

void EngineLoader::LoadEngine(const GraphicsEngineAPI type)
{
    const wchar_t * dllName = nullptr;

    if (EngineDll) {
        __framework = nullptr;
    }

    switch (type)
    {
    case GraphicsEngineAPI::OpenGL:
        dllName = Constants::DLL::opengl_graphics;
        break;
    case GraphicsEngineAPI::DirectX12:
        dllName = Constants::DLL::dx12_graphics;
        break;
    case GraphicsEngineAPI::Vulkan:
        dllName = Constants::DLL::vulkan_graphics;
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

BaseFramework* EngineLoader::GetEngine() const
{
    return __framework;
}

DLL* EngineLoader::GetEngineDll()
{
    return EngineDll.get();
}
