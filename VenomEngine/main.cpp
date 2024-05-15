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
#include "Scene.h"

#include <common/VenomEngine.h>

int main()
{
	VenomEngine engine;
	Venom::ErrorCode err;
	if ((err = engine.LoadEngine("Config.yaml")) != Venom::ErrorCode::Success)
        return (int)err;
	if ((err = engine.LoadScene<MainScene>()) != Venom::ErrorCode::Success)
		return (int)err;
	if ((err = engine.Init()) != Venom::ErrorCode::Success)
        return (int)err;
	if ((err = engine.Run()) != Venom::ErrorCode::Success)
		return (int)err;
	return (int)Venom::ErrorCode::Success;
}