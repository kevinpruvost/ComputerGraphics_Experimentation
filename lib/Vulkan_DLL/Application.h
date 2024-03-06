#pragma once

#include "ApplicationTools.h"

class Application : public ApplicationTools
{
public:
    Application();
    ErrorCode Run() override;

private:
    void initWindow();
    void initVulkan();

    void createInstance();

    void mainLoop();
    void cleanup();

    std::vector<const char *> getRequiredExtensions();

    void checkAvailableExtensions();

    WindowSettings __wsettings;
    EngineSettings __esettings;
};

