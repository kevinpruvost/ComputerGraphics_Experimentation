#include "GLFWWindow.h"

#include <common/Logger.h>
#include <common/Time.h>
#include <common/Scene.h>
#include <common/Rendering.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#ifdef _WIN32
#include <windows.h>
void glfw_onError(int error, const char* description)
{
    // print message in Windows popup dialog box
    wchar_t wdescription[1024];
    MultiByteToWideChar(CP_ACP, 0, description, -1, wdescription, 1024);
    MessageBox(NULL, (LPCSTR)wdescription, (LPCSTR)L"GLFW error", MB_OK);
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
// Framebuffer size
static std::stack<std::array<int, 2>> framebufferSizeEvents;

GLFW_ContextAPI::GLFW_ContextAPI()
    : ContextAPI(WindowAPI::GLFW)
    , __mainW{ nullptr }
    , __mainMode{ nullptr }
{
}

GLFW_ContextAPI::~GLFW_ContextAPI()
{
    keyEvents = std::stack<std::array<int, 4>>();
    keysWhileDown = std::unordered_set<InputSystem::Key>();
    mouseMoveEvents = std::stack<std::array<double, 2>>();
    Destroy();
}

// Export the factory function to create an instance of the class
EXPORT ContextAPI * createContextAPIInstance() {
    return new GLFW_ContextAPI();
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

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    framebufferSizeEvents.push({ width, height });
}

Venom::ErrorCode GLFW_ContextAPI::_Init()
{
    glfwInit();

    glfwSetErrorCallback(glfw_onError);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
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
        return Venom::ErrorCode::Failure;
    }

    __windows.push_back(w);
    __mainW = w;

    glfwMakeContextCurrent(__mainW);

    if (_settings.vsync)
        glfwSwapInterval(1);

    //glViewport(0, 0, _settings.width, _settings.height);
    glfwSetFramebufferSizeCallback(__mainW, framebuffer_size_callback);
    glfwSetKeyCallback(__mainW, key_callback);
    glfwSetCursorPosCallback(__mainW, mouse_move_callback);
    glfwSetScrollCallback(__mainW, mouse_scroll_callback);
    glfwSetMouseButtonCallback(__mainW, mouse_button_callback);
    glfwSetWindowFocusCallback(__mainW, windowFocusCallback);

    if (!_settings.iconPath.empty()) {
        SetWindowIcon(_settings.iconPath.c_str());
    }

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

    return Venom::ErrorCode::Success;
}


bool GLFW_ContextAPI::ProcessInput()
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
    // Framebuffer size events
    while (!framebufferSizeEvents.empty())
    {
        std::array<int, 2> framebufferSize = framebufferSizeEvents.top();
        if (_framebufferSizeCallback) _framebufferSizeCallback(framebufferSize[0], framebufferSize[1]);
        framebufferSizeEvents.pop();
    }
    return true;
}

Venom::ErrorCode GLFW_ContextAPI::Update()
{
    if (glfwWindowShouldClose(__mainW)) {
        SetShouldClose(true);
        return Venom::ErrorCode::Success;
    }
        // Processing input
    if (!ProcessInput()) {
        Logger::Print("Failed to process input");
        return Venom::ErrorCode::Failure;
    }

    glfwSwapBuffers(__mainW);

    Rendering::ClearColorBuffer({ 0.2f, 0.3f, 0.3f, 1.0f });
    Rendering::ClearDepthBuffer();

    // Poll events
    Time::SetNewLambda();
    glfwPollEvents();
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode GLFW_ContextAPI::Destroy()
{
    if (!__windows.empty()) {
        for (auto& w : __windows) {
            glfwDestroyWindow(w);
        }
        glfwTerminate();
    }
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode GLFW_ContextAPI::CloseWindow()
{
    glfwSetWindowShouldClose(__mainW, true);
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode GLFW_ContextAPI::MinimizeWindow()
{
    glfwIconifyWindow(__mainW);
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode GLFW_ContextAPI::SetWindowSize(int width, int height)
{
    glfwSetWindowSize(__mainW, width, height);
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode GLFW_ContextAPI::SetWindowPosition(int x, int y)
{
    glfwSetWindowPos(__mainW, x, y);
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode GLFW_ContextAPI::SetWindowTitle(const char* title)
{
    glfwSetWindowTitle(__mainW, title);
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode GLFW_ContextAPI::SetWindowIcon(const char * iconPath)
{
    int width, height, channels;
    unsigned char* image_stb = stbi_load(iconPath, &width, &height, &channels, 0);
    if (!image_stb) {
        Logger::Print("Failed to load image: %s", iconPath);
        return Venom::ErrorCode::Failure;
    }
    GLFWimage image;
    image.width = width;
    image.height = height;
    image.pixels = image_stb;
    glfwSetWindowIcon(__mainW, 1, &image);
    stbi_image_free(image_stb);
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode GLFW_ContextAPI::SetWindowVSync(VSyncModes mode)
{
    glfwSwapInterval((int)mode);
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode GLFW_ContextAPI::SetWindowFullscreen(bool enabled, int monitorIndex)
{
    int count = 0;
    GLFWmonitor ** monitors = glfwGetMonitors(&count);
    if (monitorIndex < 0 || monitorIndex >= count) {
        throw RuntimeException("Invalid monitor index: {}", monitorIndex);
    }
    int x, y;
    glfwGetWindowPos(__mainW, &x, &y);
    glfwSetWindowMonitor(__mainW, enabled ? monitors[monitorIndex] : nullptr, x, y, __mainMode->width, __mainMode->height, __mainMode->refreshRate);
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode GLFW_ContextAPI::SetWindowBorderless(bool enabled)
{
    glfwSetWindowMonitor(__mainW, glfwGetPrimaryMonitor(), 0, 0, __mainMode->width, __mainMode->height, __mainMode->refreshRate);
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode GLFW_ContextAPI::SetWindowVideoMode(const VideoMode& videoMode, int monitorIndex)
{
    __mainMode->width = videoMode.width;
    __mainMode->height = videoMode.height;
    __mainMode->refreshRate = videoMode.refreshRate;
    __mainMode->redBits = __mainMode->greenBits = __mainMode->blueBits = videoMode.bitsPerPixel / 3;
    glfwSetWindowMonitor(__mainW, glfwGetMonitors(nullptr)[monitorIndex], 0, 0, videoMode.width, videoMode.height, videoMode.refreshRate);
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode GLFW_ContextAPI::SetWindowResizable(bool enabled)
{
    glfwSetWindowAttrib(__mainW, GLFW_RESIZABLE, enabled);
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode GLFW_ContextAPI::SetWindowFocused(bool focused)
{
    glfwSetWindowAttrib(__mainW, GLFW_FOCUSED, focused);
    return Venom::ErrorCode::Success;
}

int GLFW_ContextAPI::GetWindowWidth() const
{
    int width;
    glfwGetWindowSize(__mainW, &width, nullptr);
    return width;
}

int GLFW_ContextAPI::GetWindowHeight() const
{
    int height;
    glfwGetWindowSize(__mainW, nullptr, &height);
    return height;
}

std::array<int, 2> GLFW_ContextAPI::GetWindowSize() const
{
    std::array<int, 2> size;
    glfwGetWindowSize(__mainW, &size[0], &size[1]);
    return size;
}

void GLFW_ContextAPI::LockCursor()
{
    glfwSetInputMode(__mainW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void GLFW_ContextAPI::UnlockCursor()
{
    glfwSetInputMode(__mainW, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    lastX = -1.0f;
}

GLFWwindow* GLFW_ContextAPI::GetWindow()
{
    return __mainW;
}