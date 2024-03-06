#pragma once

#include <common/Window.h>

class WinNativeWindow : public Window
{
public:
    WinNativeWindow();

protected:
    virtual ErrorCode _Init();

    virtual ErrorCode Loop();

    virtual ErrorCode Destroy();
};

