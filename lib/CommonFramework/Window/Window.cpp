#include <common/Window.h>

//#include "GLFWWindow.h"
//#include "WinNativeWindow.h"

UPtr<DLL> Window::__dll(nullptr);

Window::~Window()
{
}

Window* Window::CreateWindowFromAPI(const WindowAPI& api)
{
    const wchar_t* dllName = nullptr;

    switch (api)
    {
        case WindowAPI::GLFW:
            dllName = L"GLFWWindow.dll";
            break;
        case WindowAPI::WindowsNative:
            dllName = L"WinNativeWindow.dll";
            break;
        default:
            throw NotImplementedException("Unknown window API");
    }

    __dll.reset(new DLL(dllName));

    typedef Window* (*CreateWindowFn)();

    CreateWindowFn createFramework = __dll->getFunction<CreateWindowFn>("createWindowInstance");
    Window* window = nullptr;
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

Window::Window(const WindowAPI& api)
    : InputSystem()
    , __api{ api }
    , _sceneLoopCallback{ nullptr }
    , _appLoopCallback { nullptr }
    , _framebufferSizeCallback { nullptr }
{
}

void Window::SetSceneLoopCallback(CallbackContainer<void> callback)
{
    _sceneLoopCallback = callback;
}

void Window::SetSceneLoopCallback(Callback<void> callback)
{
    _sceneLoopCallback = callback;
}

ErrorCode Window::Init(const Config& config)
{
    _settings = config.WindowSettings();
    return _Init();
}

void Window::SetApplicationLoopCallback(CallbackContainer<void> callback)
{
    _appLoopCallback = callback;
}

void Window::SetApplicationLoopCallback(Callback<void> callback)
{
    _appLoopCallback = callback;
}

void Window::SetFramebufferSizeCallback(CallbackContainer<void, int, int> callback)
{
    _framebufferSizeCallback = callback;
}

void Window::SetFramebufferSizeCallback(Callback<void, int, int> callback)
{
    _framebufferSizeCallback = callback;
}