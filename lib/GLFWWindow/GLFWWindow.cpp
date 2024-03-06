#include "GLFWWindow.h"

GLFWWindow::GLFWWindow()
    : Window(Window::WindowAPI::GLFW)
{
}

GLFWWindow::~GLFWWindow()
{
    Destroy();
}

// Export the factory function to create an instance of the class
EXPORT Window * createWindowInstance() {
    return new GLFWWindow();
}

ErrorCode GLFWWindow::_Init()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // Vulkan needs that
    //glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, _settings.resizable);
    glfwWindowHint(GLFW_FOCUSED, _settings.focused);

    GLFWvidmode videoMode;
    videoMode.redBits = videoMode.greenBits = videoMode.blueBits = 8;
    videoMode.width = _settings.width;
    videoMode.height = _settings.height;
    videoMode.refreshRate = _settings.refreshRate;

    glfwWindowHint(GLFW_RED_BITS, videoMode.redBits);
    glfwWindowHint(GLFW_GREEN_BITS, videoMode.greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, videoMode.blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, videoMode.refreshRate);

    GLFWwindow* w = glfwCreateWindow(_settings.width, _settings.height, _settings.name.c_str(), _settings.fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
    if (w == nullptr)
    {
        Logger::Log("Failed to create window");
        glfwTerminate();
        return ErrorCode::Failure;
    }

    __windows.push_back(w);
    __mainW = w;

    glfwMakeContextCurrent(__mainW);

    //glViewport(0, 0, _settings.width, _settings.height);
    glfwSetFramebufferSizeCallback(__mainW, [](GLFWwindow* w, int width, int height) {
        glViewport(0, 0, width, height);
    });

    Logger::DebugPrint(
        "GLFW initialized with parameters :\n"
        "- Window Name: %s\n"
        "- Width: %d\n"
        "- Height: %d\n"
        "- Fullscreen: %d\n"
        "- Resizable: %d\n"
        "- Focused: %d\n"
        "- Refresh Rate: %d",
        _settings.name.c_str(), _settings.width, _settings.height, _settings.fullscreen, _settings.resizable, _settings.focused, _settings.refreshRate
    );

    return ErrorCode::Success;
}

ErrorCode GLFWWindow::Loop()
{
    while (!glfwWindowShouldClose(__mainW)) {
        // Processing input
        if (glfwGetKey(__mainW, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(__mainW, true);
        }

        _loopCallback();

        // Display
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(__mainW);

        // Poll events
        glfwPollEvents();
    }
    return ErrorCode::Success;
}

ErrorCode GLFWWindow::Destroy()
{
    if (!__windows.empty()) {
        for (auto& w : __windows) {
            glfwDestroyWindow(w);
        }
        glfwTerminate();
    }
    return ErrorCode::Success;
}
