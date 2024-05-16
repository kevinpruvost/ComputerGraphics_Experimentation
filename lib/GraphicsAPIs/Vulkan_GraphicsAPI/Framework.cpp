#include "Framework.h"

#include <common/Logger.h>

#include "Application.h"

Framework::~Framework()
{
    Destroy();
}

bool Framework::Init()
{
    _app = new Application();

    Logger::Initialize(DEFAULT_LOG_FILE_NAME);
    return true;
}

void Framework::Destroy()
{
    return;
}

int Framework::Launch()
{
    try {
        _app->Run();
    }
    catch (const std::exception& e) {
        Logger::Log(e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

Logger* Framework::GetLogger()
{
    return Logger::Get();
}

Config* Framework::GetConfig()
{
    return Config::Get(CONFIG_FILE_NAME);
}

// Export the factory function to create an instance of the class
EXPORT BaseFramework* createFrameworkInstance() {
    return new Framework();
}