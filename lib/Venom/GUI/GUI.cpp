#include <common/GUI.h>
#include <common/ShaderPipeline.h>

#include <glm/gtc/type_ptr.hpp>

UPtr<DLL> GUI::__dll(nullptr);

GUI* GUI::CreateGUIFromAPI(const WindowAPI windowApi, const GraphicsEngineAPI frameworkApi)
{
    const wchar_t* dllName = nullptr;

    if (windowApi == WindowAPI::GLFW)
    {
        if (frameworkApi == GraphicsEngineAPI::OpenGL)
            dllName = Constants::DLL::gui_opengl_glfw;
        else if (frameworkApi == GraphicsEngineAPI::Vulkan)
            dllName = Constants::DLL::gui_vulkan_glfw;
        else if (frameworkApi == GraphicsEngineAPI::DirectX12)
            dllName = Constants::DLL::gui_dx12_glfw;
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
    , _style{ nullptr }
{
}

Venom::ErrorCode GUI::Init()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    SetStyle();

    // Load a font
    io.Fonts->AddFontFromFileTTF("./resources/Fonts/satoshi.ttf", 16.0f); // Load font with size 16

    // Set the loaded font as the default font
    io.FontDefault = io.Fonts->Fonts.back();

    // Adjust the main menu bar height
    // style.FramePadding.y = 10.0f; // Increase the vertical padding
    // style.FrameRounding = 0.0f;   // Optional: Set rounding to 0 for sharper corners

    return _Init();
}

void GUI::SetEngineAndWindowForInit(ContextAPI* window, BaseFramework* engine)
{
    _window = window;
    _engine = engine;
}

void GUI::DrawMainMenuBar()
{
    // Create a menu bar
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { _style->FramePadding.x, 8.0f });

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New")) {
                // Handle "New" menu item click
            }
            if (ImGui::MenuItem("Open")) {
                // Handle "Open" menu item click
            }
            if (ImGui::MenuItem("Save")) {
                // Handle "Save" menu item click
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Cut")) {
                // Handle "Cut" menu item click
            }
            if (ImGui::MenuItem("Copy")) {
                // Handle "Copy" menu item click
            }
            if (ImGui::MenuItem("Paste")) {
                // Handle "Paste" menu item click
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGui::PopStyleVar();
}

void GUI::DrawObjectsProperties()
{
    if (!ImGui::CollapsingHeader("Object Properties", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
        return;
    std::vector<Entity *> * objects = EntityPool::GetAllEntities();
    // Draws the properties of all objects
    static int item_current_idx = -1;
    if (ImGui::BeginListBox("##Objects", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
    {
        for (int i = 0; i < objects->size(); ++i)
        {
            const bool is_selected = (item_current_idx == i);
            if (ImGui::Selectable(objects->at(i)->GetObjectName(), is_selected))
            {
                if (item_current_idx == i)
                    item_current_idx = -1;
                else
                    item_current_idx = i;
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }

    if (item_current_idx != -1)
    {
        // Move the next window to the upper right corner of the screen
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 420, 20), ImGuiCond_Once);
        // Set the size of the window
        ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_Once);

        // Begin a new window for drawing object properties
        if (ImGui::Begin("Object Properties Window", nullptr, ImGuiWindowFlags_NoCollapse))
        {
            DrawEntityProperties(&objects->at(item_current_idx));
        }
        ImGui::End(); // End the window
    }
}

void GUI::DrawEntityProperties(Entity** obj)
{
    ImGui::Text("%s:", (*obj)->GetObjectName());
    std::unordered_map<int, Component*> * components = (*obj)->GetComponents();
    for (auto pair : (*components))
    {
        auto component = pair.second;
        if (ImGui::TreeNode(component->GetComponentName()))
        {
            DrawComponentProperties(&component);
            ImGui::TreePop();
        }
    }
}

void GUI::DrawComponentProperties(Component** obj)
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
                ImGui::DragFloat3(prop.name, glm::value_ptr(*prop.vec3), 0.01f);
                break;
            case PropertyManager::Property::Type::VEC4:
                ImGui::DragFloat4(prop.name, glm::value_ptr(*prop.vec4), 0.01f);
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
            case PropertyManager::Property::Type::ENTITY_PTR:
            {
                if (ImGui::TreeNode(prop.name))
                {
                    if (*prop.entity != nullptr)
                        DrawEntityProperties(prop.entity);
                    else
                        ImGui::Text("None");
                    ImGui::TreePop();
                }
                break;
            }
            case PropertyManager::Property::Type::COMPONENT_PTR:
            {
                if (ImGui::TreeNode(prop.name))
                {
                    if (*prop.component != nullptr)
                        DrawComponentProperties(prop.component);
                    else
                        ImGui::Text("None");
                    ImGui::TreePop();
                }
                break;
            }
            }
        }
    }
}

void GUI::DrawEngineObjectProperties(const char * name, EngineObject** obj)
{    
    EngineObject::EngineObjectType type = (*obj)->GetEngineObjectType();
    switch ((*obj)->GetEngineObjectType())
    {
        case EngineObject::EngineObjectType::Resource:
        {
            EngineResource* resource = reinterpret_cast<EngineResource*>(*obj);
            auto& resources = resource->GetResourcesOfSameType();
            // Create dropdown
            std::string comboName(name); comboName += "##combo";
            if (ImGui::BeginCombo(comboName.c_str(), resource->GetResourceName())) {
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

            EngineResource::ResourceType resType = resource->GetResourceType();
            switch (resType)
            {
                case EngineResource::ResourceType::SHADER: {
                    ShaderPipeline* shader = reinterpret_cast<ShaderPipeline*>(resource);
                    shader->SetDefaultValuesForUniformVariables();
                    auto & vars = shader->GetUniformVariables();
                    shader->Use();
                    for (auto& var : vars)
                    {
                        // Only variables with mod_ as prefix for their name can be modified
                        if (strncmp(var.first, "mod_", 4) != 0)
                            continue;
                        switch (var.second.type)
                        {
                            case ShaderPipeline::UniformVariable::Type::FLOAT:
                                if (ImGui::DragFloat(var.first, &var.second.f))
                                    shader->SetUniformFloat(var.first, var.second.f);
                                break;
                            case ShaderPipeline::UniformVariable::Type::INT:
                                if (ImGui::DragInt(var.first, &var.second.i))
                                    shader->SetUniformInt(var.first, var.second.i);
                                break;
                            case ShaderPipeline::UniformVariable::Type::VEC3:
                                if (ImGui::DragFloat3(var.first, glm::value_ptr(var.second.vec3), 0.01f))
                                    shader->SetUniformVec3(var.first, var.second.vec3);
                                break;
                            case ShaderPipeline::UniformVariable::Type::VEC4:
                                if (ImGui::DragFloat4(var.first, glm::value_ptr(var.second.vec4), 0.01f))
                                    shader->SetUniformVec4(var.first, var.second.vec4);
                                break;
                            //case ShaderPipeline::UniformVariable::Type::MAT3:
                            //    ImGui::DragFloat3(var.name, &(*var.mat3)[0][0]);
                            //    ImGui::DragFloat3(var.name, &(*var.mat3)[1][0]);
                            //    ImGui::DragFloat3(var.name, &(*var.mat3)[2][0]);
                            //    break;
                            //case ShaderPipeline::UniformVariable::Type::MAT4:
                            //    ImGui::DragFloat4(var.name, &(*var.mat4)[0][0]);
                            //    ImGui::DragFloat4(var.name, &(*var.mat4)[1][0]);
                            //    ImGui::DragFloat4(var.name, &(*var.mat4)[2][0]);
                            //    ImGui::DragFloat4(var.name, &(*var.mat4)[3][0]);
                                break;
                            default:
                                break;
                        }
                    }
                    break;
                }
                default: {
                    break;
                }
            }
            break;
        }
        default:
        {
            break;
        }
    }
}

void GUI::SetStyle()
{
    _style = &ImGui::GetStyle();

    _style->WindowPadding = ImVec2(15, 15);
    _style->WindowRounding = 5.0f;
    _style->FramePadding = ImVec2(5, 5);
    _style->FrameRounding = 4.0f;
    _style->ItemSpacing = ImVec2(12, 8);
    _style->ItemInnerSpacing = ImVec2(8, 6);
    _style->IndentSpacing = 25.0f;
    _style->ScrollbarSize = 15.0f;
    _style->ScrollbarRounding = 9.0f;
    _style->GrabMinSize = 5.0f;
    _style->GrabRounding = 3.0f;

    _style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
    _style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    _style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    _style->Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    _style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    _style->Colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    _style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    _style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    _style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    _style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    _style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    _style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
    _style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    _style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    _style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    _style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    _style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    _style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    _style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    _style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    _style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    _style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    _style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    _style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    _style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    _style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    _style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    _style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    _style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    _style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    _style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    _style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    _style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    _style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    _style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
}
