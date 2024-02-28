#include "Framework.h"

#include <common/Logger.h>

bool Framework::Init()
{
    Config::Initialize(CONFIG_FILE_NAME);
    Logger::Initialize(DEFAULT_LOG_FILE_NAME);
    Logger::Print("test");
    Logger::Log("Log_test");
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

Logger* Framework::GetLogger()
{
    return Logger::Get();
}

Config* Framework::GetConfig()
{
    return Config::Get();
}

// Export the factory function to create an instance of the class
EXPORT BaseFramework* createFrameworkInstance() {
    return new Framework();
}