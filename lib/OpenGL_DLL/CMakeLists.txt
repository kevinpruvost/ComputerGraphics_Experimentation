# CMakeList.txt : CMake project for VenomEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project (OpenGL_DLL)

# Set source files for the library
file(GLOB SOURCES "*.cpp" "*.h")

add_library(OpenGL_DLL SHARED ${SOURCES})

set_target_properties(OpenGL_DLL
    PROPERTIES FOLDER "GraphicsAPIs"
)

target_link_libraries(OpenGL_DLL CommonFramework glad stb freetype)

target_include_directories(OpenGL_DLL PUBLIC "./")

include(${GRAPHICS_DLL_PROPS_PATH})