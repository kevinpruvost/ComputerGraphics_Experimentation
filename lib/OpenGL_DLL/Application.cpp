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

Venom::ErrorCode Application::Update()
{
    return Venom::ErrorCode::Success;
}

void Application::Terminate()
{
}
