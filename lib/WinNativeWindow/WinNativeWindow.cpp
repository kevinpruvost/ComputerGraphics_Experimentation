#include "WinNativeWindow.h"

WinNativeWindow::WinNativeWindow()
    : Window(WindowAPI::WindowsNative)
{
}

ErrorCode WinNativeWindow::_Init()
{
    return ErrorCode::Success;
}

ErrorCode WinNativeWindow::Loop()
{
    return ErrorCode::Success;
}

ErrorCode WinNativeWindow::Destroy()
{
    return ErrorCode::Success;
}

// Export the factory function to create an instance of the class
EXPORT Window* createWindowInstance() {
    return new WinNativeWindow();
}