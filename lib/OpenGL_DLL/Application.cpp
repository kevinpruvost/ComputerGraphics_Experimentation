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

Venom::ErrorCode Application::Run()
{
    if (Initialize() != Venom::ErrorCode::Success ||
        Loop()       != Venom::ErrorCode::Success)
        return Venom::ErrorCode::Failure;
    Terminate();
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode Application::Initialize()
{
    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    return Venom::ErrorCode::Success;
}

Venom::ErrorCode Application::Loop()
{
    _w->SetApplicationLoopCallback([&]() {
        // Display
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    });
    _w->Loop();
    return Venom::ErrorCode::Success;
}

void Application::Terminate()
{
}
