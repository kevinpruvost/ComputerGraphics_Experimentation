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
        constexpr const wchar_t * gui_dx12_glfw   = L"GUI_DX12_GLFW.dll";

        constexpr const wchar_t * glfw_window  = L"GLFW_ContextAPI.dll";
        constexpr const wchar_t * win32_window = L"WinNative_ContextAPI.dll";

        constexpr const wchar_t* opengl_graphics = L"OpenGL_GraphicsAPI.dll";
        constexpr const wchar_t* vulkan_graphics = L"Vulkan_GraphicsAPI.dll";
        constexpr const wchar_t* dx12_graphics   = L"DX12_GraphicsAPI.dll";
    }

    namespace OpenGL
    {
        constexpr const char * glsl_version = "#version 330";
    }
}