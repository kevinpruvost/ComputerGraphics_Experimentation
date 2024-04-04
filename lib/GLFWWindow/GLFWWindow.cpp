#include "GLFWWindow.h"

#include <common/Logger.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#ifdef _WIN32
#include <windows.h>
void glfw_onError(int error, const char* description)
{
    // print message in Windows popup dialog box
    wchar_t wdescription[1024];
    MultiByteToWideChar(CP_ACP, 0, description, -1, wdescription, 1024);
    MessageBox(NULL, wdescription, L"GLFW error", MB_OK);
}
#else
void glfw_onError(int error, const char* description)
{
    Logger::Log("GLFW error: {}", description);
}
#endif

#include <stack>
// Key events
// Key, scancode, action, mods
static std::stack<std::array<int, 4>> keyEvents;
// Keys that are currently down
static std::unordered_set<InputSystem::Key> keysWhileDown;

// Mouse events
// Move
static std::stack<std::array<double, 2>> mouseMoveEvents;
// Scroll
static std::stack<std::array<double, 2>> mouseScrollEvents;
// Button
static std::stack<std::array<int, 3>> mouseButtonEvents;
static std::unordered_set<InputSystem::MouseButton> buttonsWhileDown;

GLFWWindow::GLFWWindow()
    : Window(WindowAPI::GLFW)
{
}

GLFWWindow::~GLFWWindow()
{
    keyEvents = std::stack<std::array<int, 4>>();
    keysWhileDown = std::unordered_set<InputSystem::Key>();
    mouseMoveEvents = std::stack<std::array<double, 2>>();
    Destroy();
}

// Export the factory function to create an instance of the class
EXPORT Window * createWindowInstance() {
    return new GLFWWindow();
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    keyEvents.push({ key, scancode, action, mods });
}

static double lastX = -1.0f, lastY = -1.0f;
static void mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (lastX == -1.0f)
    {
        lastX = xpos;
        lastY = ypos;
    }
    mouseMoveEvents.push({ xpos - lastX, lastY - ypos });
    lastX = xpos;
    lastY = ypos;
}

static void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    mouseScrollEvents.push({ xoffset, yoffset });
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    mouseButtonEvents.push({ button, action, mods });
}

static void windowFocusCallback(GLFWwindow* window, int focused)
{
    if (focused)
    {
        lastX = -1.0f;
        Logger::DebugPrint("Window focused");
    }
    else
    {
        Logger::DebugPrint("Window unfocused");
    }
}

ErrorCode GLFWWindow::_Init()
{
    glfwInit();

    glfwSetErrorCallback(glfw_onError);

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

    glfwSetKeyCallback(__mainW, key_callback);
    glfwSetCursorPosCallback(__mainW, mouse_move_callback);
    glfwSetScrollCallback(__mainW, mouse_scroll_callback);
    glfwSetMouseButtonCallback(__mainW, mouse_button_callback);
    glfwSetWindowFocusCallback(__mainW, windowFocusCallback);


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


bool GLFWWindow::ProcessInput()
{
    // Key events
    while (!keyEvents.empty())
    {
        // Key, scancode, action, mods
        std::array<int, 4> keyEvent = keyEvents.top();
        InputSystem::Key key = __TranslateKey(keyEvent[0]);
        InputSystem::KeyModifier mods = __TranslateKeyModifier(keyEvent[3]);
        switch (keyEvent[2])
        {
            case GLFW_PRESS: {
                Logger::DebugPrint("Key pressed: %d", key);
                if (_callbackKeyDown) _callbackKeyDown(key, mods);
                keysWhileDown.insert(key);
                break;
            }
            case GLFW_RELEASE: {
                Logger::DebugPrint("Key released: %d", key);
                if (_callbackKeyUp) _callbackKeyUp(key, mods);
                keysWhileDown.erase(key);
                break;
            }
            case GLFW_REPEAT: {
                Logger::DebugPrint("Key repeated: %d", key);
                if (_callbackKeyRepeat) _callbackKeyRepeat(key, mods);
                break;
            }
        }
        keyEvents.pop();
    }
    int shiftState = glfwGetKey(__mainW, GLFW_KEY_LEFT_SHIFT) | glfwGetKey(__mainW, GLFW_KEY_RIGHT_SHIFT);
    int ctrlState = glfwGetKey(__mainW, GLFW_KEY_LEFT_CONTROL) | glfwGetKey(__mainW, GLFW_KEY_RIGHT_CONTROL);
    int altState = glfwGetKey(__mainW, GLFW_KEY_LEFT_ALT) | glfwGetKey(__mainW, GLFW_KEY_RIGHT_ALT);
    InputSystem::KeyModifier mods = __TranslateKeyModifier(shiftState, ctrlState, altState);
    for (auto key : keysWhileDown)
    {
        if (_callbackWhileKeyDown)
            _callbackWhileKeyDown(key, mods);
    }

    // Mouse move events
    while (!mouseMoveEvents.empty())
    {
        std::array<double, 2> mouseEvent = mouseMoveEvents.top();
        if (_callbackMouseMove) _callbackMouseMove(mouseEvent[0], mouseEvent[1]);
        mouseMoveEvents.pop();
    }
    // Mouse scroll events
    while (!mouseScrollEvents.empty())
    {
        std::array<double, 2> mouseEvent = mouseScrollEvents.top();
        if (_callbackMouseWheel) _callbackMouseWheel(mouseEvent[0], mouseEvent[1]);
        mouseScrollEvents.pop();
    }
    // Mouse button events
    while (!mouseButtonEvents.empty())
    {
        std::array<int, 3> mouseEvent = mouseButtonEvents.top();
        InputSystem::MouseButton button = (InputSystem::MouseButton)mouseEvent[0];
        InputSystem::KeyModifier mods = __TranslateKeyModifier(mouseEvent[2]);
        switch (mouseEvent[1])
        {
            case GLFW_PRESS: {
                Logger::DebugPrint("Mouse button pressed: %d", button);
                if (_callbackMouseDown) _callbackMouseDown(button, mods);
                break;
            }
            case GLFW_RELEASE: {
                Logger::DebugPrint("Mouse button released: %d", button);
                if (_callbackMouseUp) _callbackMouseUp(button, mods);
                break;
            }
            
        }
        mouseButtonEvents.pop();
    }
    for (auto button : buttonsWhileDown)
    {
        if (_callbackMouseWhileDown) _callbackMouseWhileDown(button, mods);
    }
    return true;
}

ErrorCode GLFWWindow::Loop()
{
    if (_appLoopCallback == nullptr)
        throw RuntimeException("App loop callback is not set");
    if (_sceneLoopCallback == nullptr)
        throw RuntimeException("Scene loop callback is not set");

    while (!glfwWindowShouldClose(__mainW)) {
        // Processing input
        if (!ProcessInput()) break;

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

int GLFWWindow::GetWindowWidth() const
{
    int width;
    glfwGetWindowSize(__mainW, &width, nullptr);
    return width;
}

int GLFWWindow::GetWindowHeight() const
{
    int height;
    glfwGetWindowSize(__mainW, nullptr, &height);
    return height;
}

std::array<int, 2> GLFWWindow::GetWindowSize() const
{
    std::array<int, 2> size;
    glfwGetWindowSize(__mainW, &size[0], &size[1]);
    return size;
}