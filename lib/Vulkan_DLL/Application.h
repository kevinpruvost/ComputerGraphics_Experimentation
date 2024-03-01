#pragma once

#include "ApplicationTools.h"

class Application : public ApplicationTools
{
public:
    int run();

private:
    void initWindow();
    void initVulkan();

    void createInstance();

    void mainLoop();
    void cleanup();

    std::vector<const char *> getRequiredExtensions();

    void checkAvailableExtensions();

    GLFWwindow* __w;
    WindowSettings __wsettings;
    EngineSettings __esettings;
};

