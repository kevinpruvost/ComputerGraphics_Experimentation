#include "Application.h"

#include <common/Logger.h>

int Application::run()
{
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
    return EXIT_SUCCESS;
}

void Application::initWindow()
{
    __wsettings = Config::Get()->WindowSettings();
    __esettings = Config::Get()->EngineSettings();

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, __wsettings.resizable);
    glfwWindowHint(GLFW_FOCUSED,   __wsettings.focused);

    GLFWvidmode videoMode;
    videoMode.redBits = videoMode.greenBits = videoMode.blueBits = 8;
    videoMode.width = __wsettings.width;
    videoMode.height = __wsettings.height;
    videoMode.refreshRate = __wsettings.refreshRate;

    glfwWindowHint(GLFW_RED_BITS,     videoMode.redBits);
    glfwWindowHint(GLFW_GREEN_BITS,   videoMode.greenBits);
    glfwWindowHint(GLFW_BLUE_BITS,    videoMode.blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, videoMode.refreshRate);

    __w = glfwCreateWindow(__wsettings.width, __wsettings.height, __wsettings.name.c_str(), __wsettings.fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);

    Logger::DebugPrint(
        "GLFW initialized with parameters :\n"
        "- Window Name: %s\n"
        "- Width: %d\n"
        "- Height: %d\n"
        "- Fullscreen: %d\n"
        "- Resizable: %d\n"
        "- Focused: %d\n"
        "- Refresh Rate: %d",
        __wsettings.name.c_str(), __wsettings.width, __wsettings.height, __wsettings.fullscreen, __wsettings.resizable, __wsettings.focused, __wsettings.refreshRate);
 }

void Application::initVulkan()
{
    createInstance();
#if _DEBUG
    setupDebugMessenger();
#endif
}

void Application::createInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = __wsettings.name.c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = __esettings.name.c_str();
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;

#if _DEBUG
    checkAvailableExtensions();
    if (__enableValidationLayers) {
        if (!checkValidationLayerSupport())
            throw std::runtime_error("validation layers requested, but not available!");
        createInfo.enabledLayerCount   = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
#endif

    const std::vector<const char*> extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

std::vector<const char*> Application::getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

#if _DEBUG
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

    return extensions;
}

void Application::checkAvailableExtensions()
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    Logger::DebugPrint("available extensions:\n");
    for (const auto& extension : extensions) {
        Logger::DebugPrint("\t%s", extension.extensionName);
    }
}

void Application::mainLoop()
{
    while (!glfwWindowShouldClose(__w)) {
        glfwPollEvents();
    }
}

void Application::cleanup()
{
    ApplicationToolsCleanup();

    vkDestroyInstance(_instance, nullptr);

    glfwDestroyWindow(__w);

    glfwTerminate();
}