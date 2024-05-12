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
#include <common/Entity.h>
#include <common/Time.h>
#include <common/Text2D.h>
#include "Scene.h"

int main()
{
	int errorCode = static_cast<int>(Venom::ErrorCode::Success);
	Venom::ErrorCode err;
	try
	{
		Logger::Initialize("test.txt");

		Config& config = *Config::Load("Config.yaml");
		WindowAPI windowApi = config.WindowSettings().api;
		EngineAPI engineApi = config.EngineSettings().api;

		UPtr<Window> w = Window::CreateWindowFromAPI(windowApi);
		w->Init(config);

		EngineLoader loader(engineApi);
		UPtr<BaseFramework> fw = loader.GetFramework();
		fw->Init();
		fw->SetWindow(w.get());

		UPtr<GUI> gui = GUI::CreateGUIFromAPI(windowApi, engineApi);
		gui->SetEngineAndWindowForInit(w.get(), fw.get());
		gui->Init();

		if ((err = config.LoadResources()) != Venom::ErrorCode::Success)
			return (int)err;

		UPtr<Scene> s = new MainScene(w.get(), fw.get(), gui.get());
		w->SetScene(s.get());

		fw->Launch();

		// Order of destruction has to be this way because of dependence
		s.reset();
		gui.reset();
		fw.reset();
		w.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		errorCode = static_cast<int>(Venom::ErrorCode::Failure);
	}
	return errorCode;
}