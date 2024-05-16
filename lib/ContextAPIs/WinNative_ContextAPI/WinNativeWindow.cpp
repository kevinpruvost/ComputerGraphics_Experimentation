#include "WinNativeWindow.h"

WinNative_ContextAPI::WinNative_ContextAPI()
    : Window(WindowAPI::WindowsNative)
{
}

Venom::ErrorCode WinNative_ContextAPI::_Init()
{
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode WinNative_ContextAPI::Loop()
{
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode WinNative_ContextAPI::Destroy()
{
    return Venom::ErrorCode::Success;
}

// Export the factory function to create an instance of the class
EXPORT Window* createContextAPIInstance() {
    return new WinNative_ContextAPI();
}