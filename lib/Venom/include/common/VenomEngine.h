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

    Venom::ErrorCode LaunchEngine();

    Venom::ErrorCode LoadEngine();
    Venom::ErrorCode LoadScene();
    Venom::ErrorCode ChangeGraphicsEngine(GraphicsEngineAPI engineAPI);
    Venom::ErrorCode ChangeContext(WindowAPI windowAPI);
    Venom::ErrorCode Init();
    Venom::ErrorCode Run();
    void SetConfigName(const char* configName);
    template<class SceneType>
    void SetScene() {
        __sceneConstructor = []() -> Scene* {
            return new SceneType();
        };
    }

private:
    Venom::ErrorCode ReloadGUI();
    Venom::ErrorCode LoadGUI(WindowAPI windowAPI, GraphicsEngineAPI engineAPI);
    Venom::ErrorCode LoadResources(Config* config);
    Venom::ErrorCode LoadGraphicsEngine(GraphicsEngineAPI engineAPI);
    Venom::ErrorCode LoadContext(WindowAPI windowAPI, const WindowSettings& windowSettings);

private:
    UPtr<ContextAPI> __w;
    UPtr<BaseFramework> __fw;
    UPtr<GUI> __gui;
    UPtr<Scene> __s;

    Scene* (*__sceneConstructor)();

    GraphicsEngineAPI __engineAPI;
    WindowAPI __windowAPI;
    const char* __configPath;
};