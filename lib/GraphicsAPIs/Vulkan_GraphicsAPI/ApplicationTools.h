#pragma once

#include "ApplicationBase.h"

class ApplicationTools : public ApplicationBase
{
public:
    ApplicationTools();
protected:
    void ApplicationToolsCleanup();

// DEBUG TOOLS
#ifdef _DEBUG
    void setupDebugMessenger();
    bool checkValidationLayerSupport();
#endif // _DEBUG

protected:
#ifdef _DEBUG
    VkDebugUtilsMessengerEXT _debugMessenger;

    static constexpr const std::array<const char*, 1> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
#else

#endif
};
