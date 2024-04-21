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

ErrorCode Application::Run()
{
    if (Initialize() != ErrorCode::Success ||
        Loop() != ErrorCode::Success)
        return ErrorCode::Failure;
    Terminate();
    return ErrorCode::Success;
}

ErrorCode Application::Initialize()
{
    
    return ErrorCode();
}

ErrorCode Application::Loop()
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
    return ErrorCode::Success;
}

void Application::Terminate()
{
    glfwTerminate();
}
