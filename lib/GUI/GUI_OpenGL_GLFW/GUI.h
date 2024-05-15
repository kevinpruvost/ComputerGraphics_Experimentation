#pragma once

#include <common/GUI.h>

class GUI_OGL_GLFW : public GUI
{
public:
    ~GUI_OGL_GLFW();
    Venom::ErrorCode _Init() override;
    void Destroy() override;
    void NewFrame() override;
    void RenderDrawData(ImDrawData* drawData) override;
    //virtual void Begin() = 0;
    //virtual bool Text(const char* txt) = 0;
    //virtual bool Checkbox(const char* txt, bool* boolRef) = 0;
    //virtual bool SliderFloat(const char* txt, float* floatRef, float min, float max) = 0;
    //virtual bool ColorEdit3(const char* txt, float* colorRef) = 0;

private:
};

extern "C"
{
    EXPORT GUI * createGUIInstance();
}