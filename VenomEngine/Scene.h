#pragma once

#include <common/FrameworkLoader.h>
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
#include <common/Entity.h>
#include <common/Time.h>
#include <common/Text2D.h>

class Scene
{
public:
    static Scene * CreateScene(Window* window, BaseFramework* framework, GUI* g);
    virtual void Update() = 0;
protected:
    Scene(Window* window, BaseFramework* framework, GUI* g);

    Window* w;
    BaseFramework* fw;
    GUI* gui;
};