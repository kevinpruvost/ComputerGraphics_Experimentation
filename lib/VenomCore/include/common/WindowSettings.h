#pragma once

#include <string>

enum class WindowAPI : int
{
    GLFW = 0,
    WindowsNative = 1
};

struct WindowSettings
{
    WindowAPI api;
    std::string name;
    std::string iconPath;
    int width;
    int height;
    bool resizable;
    bool fullscreen;
    bool vsync;
    bool focused;
    int refreshRate;
};

