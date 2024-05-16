#pragma once

#include <string>

enum class GraphicsEngineAPI : int
{
    OpenGL,
    DirectX12,
    Vulkan
};

struct EngineSettings
{
    std::string name;
    GraphicsEngineAPI api;
};