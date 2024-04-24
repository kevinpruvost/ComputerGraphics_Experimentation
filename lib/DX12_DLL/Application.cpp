#include "Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Application::Application(const Config& config)
    : ApplicationTools(config)
{
}

Application::~Application()
{
}

Venom::ErrorCode Application::Run()
{
    if (Initialize() != Venom::ErrorCode::Success ||
        Loop() != Venom::ErrorCode::Success)
        return Venom::ErrorCode::Failure;
    Terminate();
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode Application::Initialize()
{
    
    return Venom::ErrorCode();
}

Venom::ErrorCode Application::Loop()
{
    while (!glfwWindowShouldClose(__w))
    {
        // Processing input
        if (glfwGetKey(__w, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(__w, true);
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(__w);
        glfwPollEvents();
    };
    return Venom::ErrorCode::Success;
}

void Application::Terminate()
{
    glfwTerminate();
}
