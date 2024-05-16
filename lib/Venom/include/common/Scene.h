#pragma once

#include <common/Engine/EngineLoader.h>
#include <common/Logger.h>

#include <stdexcept>
#include <iostream>

#include <common/Window.h>
#include <common/Shader.h>
#include <common/ShaderPipeline.h>
#include <common/Model.h>
#include <common/Memory.h>
#include <common/Math_Base.h>
#include <common/Camera.h>
#include <common/GUI.h>
#include <common/Texture.h>
#include <common/Components/Transform.h>
#include <common/Time.h>
#include <common/Text2D.h>
#include <common/Components/Components.h>

class Scene
{
public:
    virtual Venom::ErrorCode Update() = 0;
    virtual Venom::ErrorCode _Init() = 0;
    Venom::ErrorCode Init();

    void SetWindow(ContextAPI* window);
    void SetFramework(BaseFramework* framework);
    void SetGUI(GUI* g);

    static bool IsStarted();

protected:
    Scene();

    ContextAPI* w;
    BaseFramework* fw;
    GUI* gui;
    static bool _started;
};