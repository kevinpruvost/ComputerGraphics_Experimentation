#include "Application.h"

#include <glad/glad.h>

Application::Application()
    : ApplicationTools()
{
}

Application::~Application()
{
    Logger::DebugPrint("OpenGL Application destructor called");
}

ErrorCode Application::Run()
{
    if (Initialize() != ErrorCode::Success ||
        Loop()       != ErrorCode::Success)
        return ErrorCode::Failure;
    Terminate();
    return ErrorCode::Success;
}

ErrorCode Application::Initialize()
{
    if (!gladLoadGL())
    {
        Logger::Log("Failed to initialize GLAD");
        return ErrorCode::Failure;
    }
    return ErrorCode::Success;
}

ErrorCode Application::Loop()
{
    _w->SetLoopCallback([&]() {
        Logger::DebugPrint("Zboui");
    });
    _w->Loop();
    return ErrorCode::Success;
}

void Application::Terminate()
{
    glfwTerminate();
}
