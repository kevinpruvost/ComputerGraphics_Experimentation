# CMakeList.txt : CMake project for VenomEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project (OpenGL_GraphicsAPI)

# Set source files for the library
file(GLOB SOURCES "*.cpp" "*.h")

add_library(OpenGL_GraphicsAPI SHARED ${SOURCES})

set_target_properties(OpenGL_GraphicsAPI
    PROPERTIES FOLDER "GraphicsAPIs"
)

target_link_libraries(OpenGL_GraphicsAPI Venom glad stb freetype)

target_include_directories(OpenGL_GraphicsAPI PUBLIC "./")

include(${GRAPHICS_DLL_PROPS_PATH})