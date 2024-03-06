#include <common/Window.h>

//#include "GLFWWindow.h"
//#include "WinNativeWindow.h"

DLL* Window::__dll = nullptr;

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

    if (__dll) { delete __dll; }
    __dll = new DLL(dllName);

    typedef Window* (*CreateWindowFn)();

    CreateWindowFn createFramework = reinterpret_cast<CreateWindowFn>(__dll->getFunction("createWindowInstance"));
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
        throw DLLException("Failed to find function in DLL");
    }
    return window;
}

Window::Window(const WindowAPI& api)
    : __api{ api }
    , _loopCallback{ nullptr }
{
}

void Window::SetLoopCallback(CallbackContainer<void> callback)
{
    _loopCallback = callback;
}

void Window::SetLoopCallback(Callback<void> callback)
{
    _loopCallback = callback;
}

ErrorCode Window::Init(const Config& config)
{
    _settings = config.WindowSettings();
    return _Init();
}