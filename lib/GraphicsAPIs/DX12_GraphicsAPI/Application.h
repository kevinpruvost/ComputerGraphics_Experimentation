#pragma once

#include "ApplicationTools.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Application : public ApplicationTools
{
public:
    Application(const Config& config);
    Venom::ErrorCode Run();
    Venom::ErrorCode Initialize();
    Venom::ErrorCode Loop();
    void Terminate();

    Application();
    ~Application();

private:
    GLFWwindow* __w;
};
