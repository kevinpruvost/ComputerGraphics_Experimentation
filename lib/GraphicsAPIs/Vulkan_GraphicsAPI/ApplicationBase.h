#pragma once

#include <common/ApplicationCommonBase.h>

#include <vulkan/vulkan.h>

#include <common/Math_Base.h>


#include <iostream>
#include <functional>

class ApplicationBase : public ApplicationCommonBase
{
protected:
    ApplicationBase();
    VkInstance _instance;
};

