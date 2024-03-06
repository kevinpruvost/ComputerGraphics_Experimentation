#pragma once

#include <common/ApplicationCommonBase.h>

#include <vulkan/vulkan.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <functional>

class ApplicationBase : public ApplicationCommonBase
{
protected:
    ApplicationBase();
    VkInstance _instance;
};

