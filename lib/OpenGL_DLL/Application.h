#pragma once

#include "ApplicationTools.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Application : public ApplicationTools
{
public:
    ErrorCode run();

    Application();
    ~Application();
};
