#pragma once

#include <common/DLL.h>
#include <common/Window.h>
#include <common/Engine/EngineLoader.h>
#include <common/ECS/Entity.h>
#include <common/ECS/Component.h>
#include <imgui.h>

/**
 * @brief GUI Interface based on DearIMGUI features
 */
class VENOM_API GUI
{
public:
    static GUI * CreateGUIFromAPI(const WindowAPI windowApi, const GraphicsEngineAPI frameworkApi);
private:
    static UPtr<DLL> __dll;

public:
    GUI();
    virtual ~GUI() = default;
    Venom::ErrorCode Init();
    virtual Venom::ErrorCode _Init() = 0;
    void SetEngineAndWindowForInit(ContextAPI * window, BaseFramework * engine);
    virtual void Destroy() = 0;
    virtual void NewFrame() = 0;
    virtual void RenderDrawData(ImDrawData * drawData = nullptr) = 0;
    //virtual void BeginWindow(const char * name, ImGuiWindowFlags_ flag, bool * p_open = nullptr) = 0;
    //virtual void Render() = 0;
    ///**
    // * @brief Text GUI
    // * @param txt 
    // * @return true if interaction has occured
    // */
    //virtual bool Text(const char* txt) = 0;
    ///**
    // * @brief Checkbox
    // * @param txt 
    // * @param boolRef
    // * @return true if interaction has occured
    // */
    //virtual bool Checkbox(const char* txt, bool* boolRef) = 0;
    ///**
    // * @brief Slider float
    // * @param txt 
    // * @param floatRef 
    // * @param min 
    // * @param max 
    // * @return true if interaction has occured
    // */
    //virtual bool SliderFloat(const char* txt, float* floatRef, float min, float max) = 0;
    ///**
    // * @brief Color Edit GUI
    // * @param txt 
    // * @param colorRef MUST be a pointer towards 3 floats
    // * @return true if interaction has occured
    // */
    //virtual bool ColorEdit3(const char* txt, float* colorRef) = 0;
    void DrawMainMenuBar();
    void DrawObjectsProperties();
    void DrawEntityProperties(Entity** obj);
    void DrawComponentProperties(Component** obj);
    /**
     * @brief Draws engine object properties
     * @param name 
     * @param obj 
     * @return true if interaction has occured, false otherwise
     */
    bool DrawEngineObjectProperties(const char * name, EngineObject ** obj);
    bool DrawEngineObjectProperties(const char * name, EngineObject ** obj, EngineObject::EngineObjectType type, EngineResource::ResourceType resourceType);
    template<class T>
    inline bool DrawEngineObjectProperties(const char* name, T** obj, EngineResource::ResourceType resourceType) {
        return DrawEngineObjectProperties(name, reinterpret_cast<EngineObject**>(obj), EngineObject::EngineObjectType::Resource, resourceType);
    }
private:
    void SetStyle();

protected:
    ContextAPI * _window;
    BaseFramework* _engine;
    ImGuiStyle * _style;
};