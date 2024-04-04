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

GUI::GUI()
    : _window{ nullptr }
    , _engine{ nullptr }
{
}

void GUI::Init()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    _Init();
}

void GUI::SetEngineAndWindowForInit(Window* window, BaseFramework* engine)
{
    _window = window;
    _engine = engine;
}
