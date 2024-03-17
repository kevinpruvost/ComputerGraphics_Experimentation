#include "GLFWWindow.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

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
    if (_appLoopCallback == nullptr) {
        throw RuntimeException("App loop callback is not set");
    }
    if (_sceneLoopCallback == nullptr) {
        throw RuntimeException("Scene loop callback is not set");
    }

    while (!glfwWindowShouldClose(__mainW)) {
        // Processing input
        if (glfwGetKey(__mainW, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(__mainW, true);
        }

        // Display
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        _appLoopCallback();
        _sceneLoopCallback();

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

ErrorCode GLFWWindow::CloseWindow()
{
    glfwSetWindowShouldClose(__mainW, true);
    return ErrorCode::Success;
}

ErrorCode GLFWWindow::MinimizeWindow()
{
    glfwIconifyWindow(__mainW);
    return ErrorCode::Success;
}

ErrorCode GLFWWindow::SetWindowSize(int width, int height)
{
    glfwSetWindowSize(__mainW, width, height);
    return ErrorCode::Success;
}

ErrorCode GLFWWindow::SetWindowPosition(int x, int y)
{
    glfwSetWindowPos(__mainW, x, y);
    return ErrorCode::Success;
}

ErrorCode GLFWWindow::SetWindowTitle(const char* title)
{
    glfwSetWindowTitle(__mainW, title);
    return ErrorCode::Success;
}

ErrorCode GLFWWindow::SetWindowIcon(const std::filesystem::path & iconPath)
{
    int width, height, channels;
    unsigned char* image_stb = stbi_load(iconPath.string().c_str(), &width, &height, &channels, 0);
    if (!image_stb) {
        throw RuntimeException("Couldn't load image: {}", iconPath.string().c_str());
    }
    GLFWimage image;
    image.width = width;
    image.height = height;
    image.pixels = image_stb;
    glfwSetWindowIcon(__mainW, 1, &image);
    stbi_image_free(image_stb);
    return ErrorCode::Success;
}

ErrorCode GLFWWindow::SetWindowVSync(VSyncModes mode)
{
    glfwSwapInterval((int)mode);
    return ErrorCode::Success;
}

ErrorCode GLFWWindow::SetWindowFullscreen(bool enabled, int monitorIndex)
{
    int count = 0;
    GLFWmonitor ** monitors = glfwGetMonitors(&count);
    if (monitorIndex < 0 || monitorIndex >= count) {
        throw RuntimeException("Invalid monitor index: {}", monitorIndex);
    }
    int x, y;
    glfwGetWindowPos(__mainW, &x, &y);
    glfwSetWindowMonitor(__mainW, enabled ? monitors[monitorIndex] : nullptr, x, y, __mainMode->width, __mainMode->height, __mainMode->refreshRate);
    return ErrorCode::Success;
}

ErrorCode GLFWWindow::SetWindowBorderless(bool enabled)
{
    glfwSetWindowMonitor(__mainW, glfwGetPrimaryMonitor(), 0, 0, __mainMode->width, __mainMode->height, __mainMode->refreshRate);
    return ErrorCode::Success;
}

ErrorCode GLFWWindow::SetWindowVideoMode(const VideoMode& videoMode, int monitorIndex)
{
    __mainMode->width = videoMode.width;
    __mainMode->height = videoMode.height;
    __mainMode->refreshRate = videoMode.refreshRate;
    __mainMode->redBits = __mainMode->greenBits = __mainMode->blueBits = videoMode.bitsPerPixel / 3;
    glfwSetWindowMonitor(__mainW, glfwGetMonitors(nullptr)[monitorIndex], 0, 0, videoMode.width, videoMode.height, videoMode.refreshRate);
    return ErrorCode::Success;
}

ErrorCode GLFWWindow::SetWindowResizable(bool enabled)
{
    glfwSetWindowAttrib(__mainW, GLFW_RESIZABLE, enabled);
    return ErrorCode::Success;
}

ErrorCode GLFWWindow::SetWindowFocused(bool focused)
{
    glfwSetWindowAttrib(__mainW, GLFW_FOCUSED, focused);
    return ErrorCode::Success;
}
