#include "Framework.h"

#include <common/Logger.h>

#include "Application.h"

bool Framework::Init()
{
    _app = new Application();

    SetConfig(*Config::Load(CONFIG_FILE_NAME));
    Logger::Initialize(DEFAULT_LOG_FILE_NAME);
    return true;
}

void Framework::Destroy()
{
    if (_app) delete _app;
    return;
}

int Framework::Launch()
{
    try {
        _app->Run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
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