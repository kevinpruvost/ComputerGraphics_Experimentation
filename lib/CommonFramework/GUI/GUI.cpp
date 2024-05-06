#include <common/GUI.h>

#include <glm/gtc/type_ptr.hpp>

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

void GUI::DrawObjectsProperties()
{
    if (!ImGui::CollapsingHeader("Object Properties", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
        return;
    auto & objects = ObjectPool::GetAllObjects();
    // Draws the properties of all objects
    for (auto& obj : objects)
    {
        DrawObjectProperties(&obj);
    }
}

void GUI::DrawObjectProperties(Object** obj)
{
    if (ImGui::TreeNode((*obj)->GetObjectName()))
    {
        if (auto& cb = (*obj)->GetGUICallback())
        {
            cb();
        }
        else {
            auto& props = (*obj)->GetProperties();
            for (auto& prop : props)
            {
                switch (prop.type)
                {
                case PropertyManager::Property::Type::BOOL:
                    ImGui::Checkbox(prop.name, prop.b);
                    break;
                case PropertyManager::Property::Type::FLOAT:
                    ImGui::DragFloat(prop.name, prop.f);
                    break;
                case PropertyManager::Property::Type::INT:
                    ImGui::DragInt(prop.name, prop.i);
                    break;
                case PropertyManager::Property::Type::STRING:
                    ImGui::InputText(prop.name, prop.s, 30);
                    break;
                case PropertyManager::Property::Type::VEC3:
                    ImGui::DragFloat3(prop.name, glm::value_ptr(*prop.vec3));
                    break;
                case PropertyManager::Property::Type::VEC4:
                    ImGui::DragFloat4(prop.name, glm::value_ptr(*prop.vec4));
                    break;
                case PropertyManager::Property::Type::MAT3:
                    ImGui::DragFloat3(prop.name, &(*prop.mat3)[0][0]);
                    ImGui::DragFloat3(prop.name, &(*prop.mat3)[1][0]);
                    ImGui::DragFloat3(prop.name, &(*prop.mat3)[2][0]);
                    break;
                case PropertyManager::Property::Type::MAT4:
                    ImGui::DragFloat4(prop.name, &(*prop.mat4)[0][0]);
                    ImGui::DragFloat4(prop.name, &(*prop.mat4)[1][0]);
                    ImGui::DragFloat4(prop.name, &(*prop.mat4)[2][0]);
                    ImGui::DragFloat4(prop.name, &(*prop.mat4)[3][0]);
                    break;
                case PropertyManager::Property::Type::ENGINE_OBJECT_PTR:
                    DrawEngineObjectProperties(prop.name, prop.engineObject);
                    break;
                case PropertyManager::Property::Type::OBJECT_PTR:
                    if (ImGui::TreeNode(prop.name))
                    {
                        DrawObjectProperties(prop.object);
                        ImGui::TreePop();
                    }
                    break;
                }
            }
        }
        ImGui::TreePop();
    }
}

void GUI::DrawEngineObjectProperties(const char * name, EngineObject** obj)
{    
    switch ((*obj)->GetEngineObjectType())
    {
        case EngineObject::EngineObjectType::Resource:
        {
            EngineResource* resource = reinterpret_cast<EngineResource*>(*obj);
            ImGui::Text(name);
            auto& resources = resource->GetResourcesOfSameType();
            // Create dropdown
            if (ImGui::BeginCombo("Resource:", resource->GetResourceName())) {
                for (int i = 0; i < resources.size(); i++) {
                    bool isSelected = resources[i] == resource;
                    if (ImGui::Selectable(resources[i]->GetResourceName(), isSelected))
                    {
                        *obj = reinterpret_cast<EngineObject*>(resources[i]);
                    }

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            break;
        }
        default:
        {
            break;
        }
    }
}
