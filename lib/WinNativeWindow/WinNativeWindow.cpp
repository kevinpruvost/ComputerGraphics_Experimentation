#include "WinNativeWindow.h"

WinNativeWindow::WinNativeWindow()
    : Window(Window::WindowAPI::WindowsNative)
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
