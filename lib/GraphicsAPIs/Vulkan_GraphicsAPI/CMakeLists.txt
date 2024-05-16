# CMakeList.txt : CMake project for VenomEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project (Vulkan_GraphicsAPI)

# Set source files for the library
file(GLOB SOURCES "*.cpp" "*.h")

add_library(Vulkan_GraphicsAPI SHARED ${SOURCES})

set_target_properties(Vulkan_GraphicsAPI
    PROPERTIES FOLDER "GraphicsAPIs"
)

target_link_libraries(Vulkan_GraphicsAPI VenomCore)

include(${GRAPHICS_DLL_PROPS_PATH})