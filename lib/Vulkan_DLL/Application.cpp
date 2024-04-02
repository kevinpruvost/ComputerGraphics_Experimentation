#include "Application.h"

#include <common/Logger.h>

#include <GLFW/glfw3.h>

#include <functional>

Application::Application()
    : ApplicationTools()
{
}

ErrorCode Application::Run()
{
    initWindow();
    initVulkan();
    // Window Loop
    _w->SetApplicationLoopCallback([&]() {
    });
    _w->Loop();
    cleanup();
    return ErrorCode::Failure;
}

void Application::initWindow()
{
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
    if (!checkValidationLayerSupport())
        throw std::runtime_error("validation layers requested, but not available!");
    createInfo.enabledLayerCount   = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
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

    Logger::DebugPrint("Available extensions:\n");
    for (const auto& extension : extensions) {
        Logger::DebugPrint("\t%s", extension.extensionName);
    }
}

void Application::cleanup()
{
    ApplicationToolsCleanup();

    vkDestroyInstance(_instance, nullptr);
}