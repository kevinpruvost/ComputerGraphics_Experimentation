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
#include <common/Components/ParticleSystem.h>
#include <common/Skybox.h>

class Scene
{
public:
    virtual void Update() = 0;
protected:
    Scene(Window* window, BaseFramework* framework, GUI* g);

    Window* w;
    BaseFramework* fw;
    GUI* gui;
};