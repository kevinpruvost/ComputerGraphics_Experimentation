#pragma once

#include "ApplicationTools.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Application : public ApplicationTools
{
public:
    Application(const Config& config);
    ErrorCode Run();
    ErrorCode Initialize();
    ErrorCode Loop();
    void Terminate();

    Application();
    ~Application();

private:
    GLFWwindow* __w;
};
