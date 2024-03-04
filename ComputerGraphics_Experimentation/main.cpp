#include "FrameworkLoader.h"
#include <common/Logger.h>

#include <stdexcept>
#include <iostream>

int main()
{
	try
	{
		Logger::Initialize("test.txt");
		FrameworkLoader loader(FrameworkLoader::FrameworkType::OpenGL);
		BaseFramework* fw = loader.GetFramework();
		fw->Init();
		fw->Launch();
		fw->GetLogger()->Log("Framework launched with Vulkan\n");
		Logger::Log("Framework launched with Vulkan\n");
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}
    return 0;
}