#pragma once

/**
 * @brief Contains all constants variables
 */
namespace Constants
{
    /**
     * @brief Contains all DLL related constant variables
     */
    namespace DLL
    {
        constexpr const wchar_t * gui_opengl_glfw = L"GUI_OpenGL_GLFW.dll";
        constexpr const wchar_t * gui_vulkan_glfw = L"GUI_Vulkan_GLFW.dll";
        constexpr const wchar_t * gui_dx11_glfw   = L"GUI_DirectX11_GLFW.dll";

        constexpr const wchar_t * glfw_window  = L"GLFWWindow.dll";
        constexpr const wchar_t * win32_window = L"WinNativeWindow.dll";

        constexpr const wchar_t* opengl_graphics = L"OpenGL_DLL.dll";
        constexpr const wchar_t* vulkan_graphics = L"Vulkan_DLL.dll";
        constexpr const wchar_t* dx11_graphics   = L"DX11_DLL.dll";
    }

    namespace OpenGL
    {
        constexpr const char * glsl_version = "#version 330";
    }
}