#include <common/Window.h>
#include <common/Constants.h>

UPtr<DLL> ContextAPI::__dll(nullptr);

ContextAPI::~ContextAPI()
{
}

ContextAPI* ContextAPI::CreateWindowFromAPI(const WindowAPI& api)
{
    const wchar_t* dllName = nullptr;

    switch (api)
    {
        case WindowAPI::GLFW:
            dllName = Constants::DLL::glfw_window;
            break;
        case WindowAPI::WindowsNative:
            dllName = Constants::DLL::win32_window;
            break;
        default:
            throw NotImplementedException("Unknown window API");
    }

    __dll.reset(new DLL(dllName));

    typedef ContextAPI* (*CreateWindowFn)();

    CreateWindowFn createFramework = __dll->getFunction<CreateWindowFn>("createContextAPIInstance");
    ContextAPI* window = nullptr;
    if (createFramework != nullptr) {
        // Call function to create object
        window = createFramework();
        if (window == nullptr)
        {
            throw DLLException("Failed to create window object");
        }
    }
    else {
        // Function not found
        throw DLLException("Failed to find function in DLL: {}", (char *)dllName);
    }
    return window;
}

bool ContextAPI::ShouldClose() const
{
    return __shouldClose;
}

ContextAPI::ContextAPI(const WindowAPI& api)
    : InputSystem()
    , __api{ api }
    , _appLoopCallback { nullptr }
    , _framebufferSizeCallback { nullptr }
    , __shouldClose{ false }
{
}

void ContextAPI::SetShouldClose(bool shouldClose)
{
    __shouldClose = shouldClose;
}

Venom::ErrorCode ContextAPI::Init(const WindowSettings& windowSettings)
{
    _settings = windowSettings;
    return _Init();
}

void ContextAPI::SetApplicationLoopCallback(CallbackContainer<void> callback)
{
    _appLoopCallback = callback;
}

void ContextAPI::SetFramebufferSizeCallback(CallbackContainer<void, int, int> callback)
{
    _framebufferSizeCallback = callback;
}