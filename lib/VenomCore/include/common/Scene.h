#pragma once

#include <common/Engine/EngineLoader.h>
#include <common/Logger.h>

#include <stdexcept>
#include <iostream>

#include <common/Window.h>
#include <common/Memory.h>
#include <common/GUI.h>
#include <common/Time.h>

#include <common/Interfaces/IScene.h>

class Scene : public IScene
{
public:
    Venom::ErrorCode Init();

    void SetWindow(ContextAPI* window);
    void SetFramework(BaseFramework* framework);
    void SetGUI(GUI* g);

protected:
    Scene();

    ContextAPI* w;
    BaseFramework* fw;
    GUI* gui;
};