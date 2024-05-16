#pragma once

#include <common/Window.h>
#include <common/DLL.h>

class VENOM_CONTEXT_API WinNative_ContextAPI : public ContextAPI
{
public:
    WinNative_ContextAPI();

protected:
    virtual Venom::ErrorCode _Init();

    virtual Venom::ErrorCode Loop();

    virtual Venom::ErrorCode Destroy();
};

extern "C"
{
    VENOM_CONTEXT_API ContextAPI* createContextAPIInstance();
}