#pragma once

#include <common/Window.h>
#include <common/DLL.h>

class CONTEXT_API WinNativeWindow : public Window
{
public:
    WinNativeWindow();

protected:
    virtual ErrorCode _Init();

    virtual ErrorCode Loop();

    virtual ErrorCode Destroy();
};

extern "C"
{
    CONTEXT_API Window* createWindowInstance();
}