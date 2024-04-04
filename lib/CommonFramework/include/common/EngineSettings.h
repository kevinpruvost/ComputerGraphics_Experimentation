#pragma once

#include <string>

enum class EngineAPI : int
{
    OpenGL,
    DirectX11,
    Vulkan
};

struct EngineSettings
{
    std::string name;
    EngineAPI api;
};