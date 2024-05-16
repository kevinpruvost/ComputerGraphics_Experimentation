#include "Framework.h"

#include <common/Logger.h>

#include "Application.h"

Framework::~Framework()
{
    Destroy();
}

bool Framework::Init()
{
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
        Application app(*Config::Get());
        app.Run();
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
    return Config::Get();
}

// Export the factory function to create an instance of the class
EXPORT BaseFramework* createFrameworkInstance() {
    return new Framework();
}