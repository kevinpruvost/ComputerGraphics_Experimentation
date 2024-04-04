#include <common/GUI.h>

UPtr<DLL> GUI::__dll(nullptr);

GUI* GUI::CreateGUIFromAPI(const WindowAPI windowApi, const EngineAPI frameworkApi)
{
    const wchar_t* dllName = nullptr;

    if (windowApi == WindowAPI::GLFW)
    {
        if (frameworkApi == EngineAPI::OpenGL)
            dllName = Constants::DLL::gui_opengl_glfw;
        else if (frameworkApi == EngineAPI::Vulkan)
            dllName = Constants::DLL::gui_vulkan_glfw;
        else if (frameworkApi == EngineAPI::DirectX11)
            dllName = Constants::DLL::gui_dx11_glfw;
    }
    else if (windowApi == WindowAPI::WindowsNative)
    {
        //if (frameworkApi == FrameworkLoader::FrameworkType::OpenGL)
        //    dllName = Constants::DLL::gui_opengl_glfw;
        //else if (frameworkApi == FrameworkLoader::FrameworkType::Vulkan)
        //    dllName = Constants::DLL::gui_vulkan_glfw;
        //else if (frameworkApi == FrameworkLoader::FrameworkType::DirectX11)
        //    dllName = Constants::DLL::gui_dx11_glfw;
    }

    if (dllName == nullptr)
        throw NotImplementedException("Unknown GUI API");

    __dll.reset(new DLL(dllName));

    typedef GUI * (*CreateGUIFn)();

    CreateGUIFn createGui = __dll->getFunction<CreateGUIFn>("createGUIInstance");
    GUI * gui = nullptr;
    if (createGui != nullptr) {
        // Call function to create object
        gui = createGui();
        if (gui == nullptr)
        {
            throw DLLException("Failed to create gui object");
        }
    }
    else {
        // Function not found
        throw DLLException("Failed to find function in DLL: {}", (char*)dllName);
    }
    return gui;
}
