# CMakeList.txt : CMake project for VenomEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project (DX12_GraphicsAPI)

# Set source files for the library
file(GLOB SOURCES "*.cpp" "*.h")

add_library(DX12_GraphicsAPI SHARED ${SOURCES})

set_target_properties(DX12_GraphicsAPI
    PROPERTIES FOLDER "GraphicsAPIs"
)

target_link_libraries(DX12_GraphicsAPI Venom)

include(${GRAPHICS_DLL_PROPS_PATH})