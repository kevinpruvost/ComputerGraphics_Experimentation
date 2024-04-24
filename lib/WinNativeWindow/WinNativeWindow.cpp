#include "WinNativeWindow.h"

WinNativeWindow::WinNativeWindow()
    : Window(WindowAPI::WindowsNative)
{
}

Venom::ErrorCode WinNativeWindow::_Init()
{
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode WinNativeWindow::Loop()
{
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode WinNativeWindow::Destroy()
{
    return Venom::ErrorCode::Success;
}

// Export the factory function to create an instance of the class
EXPORT Window* createWindowInstance() {
    return new WinNativeWindow();
}