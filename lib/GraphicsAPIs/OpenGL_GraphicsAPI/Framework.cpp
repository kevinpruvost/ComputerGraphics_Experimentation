#include "Framework.h"

#include <common/Logger.h>
#include "BufferManager.h"
#include "Application.h"

Framework::~Framework()
{
    Destroy();
}

Venom::ErrorCode Framework::Init()
{
    _app = new Application();

    Logger::Initialize(DEFAULT_LOG_FILE_NAME);

    if (!gladLoadGL())
    {
        Logger::Log("Failed to initialize GLAD");
        return Venom::ErrorCode::Failure;
    }
    BufferManager::Initialize();
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode Framework::Update()
{
    return Venom::ErrorCode::Success;
}

void Framework::Destroy()
{
    if (_app) delete _app;
    BufferManager::Destroy();
    return;
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