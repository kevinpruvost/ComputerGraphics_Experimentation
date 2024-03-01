#pragma once

#include <string>

struct WindowSettings
{
    std::string name;
    int width;
    int height;
    bool resizable;
    bool fullscreen;
    bool vsync;
    bool focused;
    int refreshRate;
};

