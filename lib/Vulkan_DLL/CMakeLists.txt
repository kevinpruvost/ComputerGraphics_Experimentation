# CMakeList.txt : CMake project for VenomEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project (Vulkan_DLL)

# Set source files for the library
file(GLOB SOURCES "*.cpp" "*.h")

add_library(Vulkan_DLL SHARED ${SOURCES})

set_target_properties(Vulkan_DLL
    PROPERTIES FOLDER "GraphicsAPIs"
)

target_link_libraries(Vulkan_DLL CommonFramework)

include(${GRAPHICS_DLL_PROPS_PATH})