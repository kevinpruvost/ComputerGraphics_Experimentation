# CMakeList.txt : CMake project for VenomEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project (GUI_OpenGL_GLFW)

# Set source files for the library
file(GLOB SOURCES "*.cpp" "*.h"
    ${EXTERNAL_LIBS_PATH}imgui/backends/imgui_impl_glfw.cpp
    ${EXTERNAL_LIBS_PATH}imgui/backends/imgui_impl_glfw.h
    ${EXTERNAL_LIBS_PATH}imgui/backends/imgui_impl_opengl3.cpp
    ${EXTERNAL_LIBS_PATH}imgui/backends/imgui_impl_opengl3.h
)

add_library(GUI_OpenGL_GLFW SHARED ${SOURCES})

set_target_properties(GUI_OpenGL_GLFW
    PROPERTIES FOLDER "GUIAPIs"
)

target_include_directories(GUI_OpenGL_GLFW PRIVATE ${EXTERNAL_LIBS_PATH}imgui/backends/)

target_link_libraries(GUI_OpenGL_GLFW VenomCore GLFW_ContextAPI imgui OpenGL_GraphicsAPI glfw)