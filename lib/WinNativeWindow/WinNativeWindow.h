#pragma once

#include <common/Window.h>
#include <common/DLL.h>

class CONTEXT_API WinNativeWindow : public Window
{
public:
    WinNativeWindow();

protected:
    virtual Venom::ErrorCode _Init();

    virtual Venom::ErrorCode Loop();

    virtual Venom::ErrorCode Destroy();
};

extern "C"
{
    CONTEXT_API Window* createWindowInstance();
}