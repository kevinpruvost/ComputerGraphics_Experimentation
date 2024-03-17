#include "FrameworkLoader.h"
#include <common/Logger.h>

#include <stdexcept>
#include <iostream>

#include <common/Window.h>

int main()
{
	try
	{
		Logger::Initialize("test.txt");

		Window * w = Window::CreateWindowFromAPI(Window::WindowAPI::GLFW);
		Config & windowConfig = *Config::Load("Config_GLFWWindow.yaml");
		w->Init(windowConfig);

		FrameworkLoader::FrameworkType engineType = FrameworkLoader::FrameworkType::OpenGL;
		FrameworkLoader loader(engineType);
		BaseFramework* fw = loader.GetFramework();
		fw->Init();
		fw->SetWindow(w);
		fw->Launch();
		fw->GetLogger()->Log("Framework launched with %d\n", engineType);

		delete w;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (int)ErrorCode::Failure;
	}
    return (int)ErrorCode::Success;
}