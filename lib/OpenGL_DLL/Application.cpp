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
    return ErrorCode::Success;
}

ErrorCode Application::Loop()
{
    _w->SetApplicationLoopCallback([&]() {

    });
    _w->Loop();
    return ErrorCode::Success;
}

void Application::Terminate()
{
    glfwTerminate();
}
