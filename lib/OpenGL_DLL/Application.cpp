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
    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return ErrorCode::Success;
}

ErrorCode Application::Loop()
{
    _w->SetApplicationLoopCallback([&]() {
        // Display
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    });
    _w->Loop();
    return ErrorCode::Success;
}

void Application::Terminate()
{
}
