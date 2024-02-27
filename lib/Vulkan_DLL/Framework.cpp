#include "Framework.h"

#include <common/Logger.h>

bool Framework::Init()
{
    Logger::Initialize("Vulkan_Log.txt");
    Logger::Print("Hello Vulkan !\n");
    return true;
}


void Framework::Destroy()
{
    return;
}


void Framework::Launch()
{
    return;
}