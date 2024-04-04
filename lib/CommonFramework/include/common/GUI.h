#pragma once

#include <common/DLL.h>
#include <common/Window.h>
#include <common/FrameworkLoader.h>

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

    virtual void Init() = 0;
    virtual void Destroy() = 0;
    virtual void Begin() = 0;
    /**
     * @brief Text GUI
     * @param txt 
     * @return true if interaction has occured
     */
    virtual bool Text(const char* txt) = 0;
    /**
     * @brief Checkbox
     * @param txt 
     * @param boolRef
     * @return true if interaction has occured
     */
    virtual bool Checkbox(const char* txt, bool* boolRef) = 0;
    /**
     * @brief Slider float
     * @param txt 
     * @param floatRef 
     * @param min 
     * @param max 
     * @return true if interaction has occured
     */
    virtual bool SliderFloat(const char* txt, float* floatRef, float min, float max) = 0;
    /**
     * @brief Color Edit GUI
     * @param txt 
     * @param colorRef MUST be a pointer towards 3 floats
     * @return true if interaction has occured
     */
    virtual bool ColorEdit3(const char* txt, float* colorRef) = 0;
};