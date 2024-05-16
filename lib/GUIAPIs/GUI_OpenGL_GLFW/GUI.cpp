#include "GUI.h"

#include <common/Constants.h>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// GLFW
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>   // for glfwGetWin32Window()

#include <Framework.h>
#include <GLFWWindow.h>

GUI* createGUIInstance()
{
    return new GUI_OGL_GLFW();
}

GUI_OGL_GLFW::~GUI_OGL_GLFW()
{
    Destroy();
}

Venom::ErrorCode GUI_OGL_GLFW::_Init()
{
    // Setup Platform/Renderer backends
    if (!_window || !_engine) {
        Logger::Print("GUI didn't get window or engine instances, call GUI::SetEngineAndWindowForInit");
        return Venom::ErrorCode::Failure;
    }
    GLFW_ContextAPI* w = (GLFW_ContextAPI*)(_window);
    Framework* fw = (Framework*)(_engine);
    ImGui_ImplGlfw_InitForOpenGL(w->GetWindow(), true);
    // Constant for OpenGL
    ImGui_ImplOpenGL3_Init(Constants::OpenGL::glsl_version);
    return Venom::ErrorCode::Success;
}

void GUI_OGL_GLFW::Destroy()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI_OGL_GLFW::NewFrame()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GUI_OGL_GLFW::RenderDrawData(ImDrawData * drawData)
{
    ImGui_ImplOpenGL3_RenderDrawData(drawData ? drawData : ImGui::GetDrawData());
}
