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
#include <common/Object.h>
#include <common/Time.h>
#include <common/Text2D.h>
#include "Scene.h"

UPtr<Scene> s;

void scene()
{
	s->Update();
}

int main()
{
	try
	{
		Logger::Initialize("test.txt");

		Config& config = *Config::Load("Config.yaml");
		WindowAPI windowApi = config.WindowSettings().api;
		EngineAPI engineApi = config.EngineSettings().api;

		UPtr<Window> w = Window::CreateWindowFromAPI(windowApi);
		w->Init(config);
		w->SetSceneLoopCallback(scene);

		FrameworkLoader loader(engineApi);
		UPtr<BaseFramework> fw = loader.GetFramework();
		fw->Init();
		fw->SetWindow(w.get());

		UPtr<GUI> gui = GUI::CreateGUIFromAPI(windowApi, engineApi);
		gui->SetEngineAndWindowForInit(w.get(), fw.get());
		gui->Init();

		s = Scene::CreateScene(w.get(), fw.get(), gui.get());

		fw->Launch();
		fw->GetLogger()->Log("Framework launched with %d\n", engineApi);

		// Order of destruction has to be this way because of dependence
		s.reset();
		gui.reset();
		fw.reset();
		w.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (int)ErrorCode::Failure;
	}
    return (int)ErrorCode::Success;
}