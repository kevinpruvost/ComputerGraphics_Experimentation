#include "FrameworkLoader.h"

#include <stdexcept>
#include <iostream>

int main()
{
	try
	{
		FrameworkLoader loader(FrameworkLoader::FrameworkType::Vulkan);
		BaseFramework* framework = loader.GetFramework();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}


    return 0;
}