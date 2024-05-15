#pragma once

#include <common/Error.h>
#include <common/Window.h>
#include <common/BaseFramework.h>
#include <common/GUI.h>
#include <common/Scene.h>

class VenomEngine
{
public:
    VenomEngine();
    ~VenomEngine();

    Venom::ErrorCode LoadEngine(const char * configPath);
    template<typename SceneType>
    Venom::ErrorCode LoadScene() {
        __s = new SceneType();
        __s->SetWindow(__w.get());
        __s->SetFramework(__fw.get());
        __s->SetGUI(__gui.get());
        return Venom::ErrorCode::Success;
    }
    Venom::ErrorCode Init();
    Venom::ErrorCode Run();

private:
    UPtr<Window> __w;
    UPtr<BaseFramework> __fw;
    UPtr<GUI> __gui;
    UPtr<Scene> __s;
};