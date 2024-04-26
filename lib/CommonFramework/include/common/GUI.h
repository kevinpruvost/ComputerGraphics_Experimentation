#pragma once

#include <common/DLL.h>
#include <common/Window.h>
#include <common/Engine/EngineLoader.h>
#include <imgui.h>

/**
 * @brief GUI Interface based on DearIMGUI features
 */
class COMMONFRAMEWORK_API GUI
{
public:
    static GUI * CreateGUIFromAPI(const WindowAPI windowApi, const EngineAPI frameworkApi);
private:
    static UPtr<DLL> __dll;

public:
    GUI();
    virtual ~GUI() = default;
    void Init();
    virtual void _Init() = 0;
    void SetEngineAndWindowForInit(Window * window, BaseFramework * engine);
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
protected:
    Window * _window;
    BaseFramework* _engine;
};