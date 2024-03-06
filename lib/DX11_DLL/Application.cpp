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
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    __w = glfwCreateWindow(
        _windowSettings.width, // Window Width
        _windowSettings.height, // Window Height
        _windowSettings.name.c_str(), // Window Name
        _windowSettings.fullscreen ? glfwGetPrimaryMonitor() : nullptr, // Monitor
        nullptr // Context to share resources with
    );

    if (__w == nullptr)
    {
        Logger::Log("Failed to create window");
        glfwTerminate();
        return ErrorCode::Failure;
    }

    glfwMakeContextCurrent(__w);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Logger::Log("Failed to initialize GLAD");
        return ErrorCode::Failure;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(__w, [](GLFWwindow* w, int width, int height) {
        glViewport(0, 0, width, height);
        });
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
