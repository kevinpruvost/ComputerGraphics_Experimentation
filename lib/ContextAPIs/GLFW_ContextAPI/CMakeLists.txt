# CMakeList.txt : CMake project for VenomEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project (GLFW_ContextAPI)

# Set source files for the library
file(GLOB SOURCES "*.cpp" "*.h")

add_library(GLFW_ContextAPI SHARED ${SOURCES})

set_target_properties(GLFW_ContextAPI
    PROPERTIES FOLDER "ContextAPIs"
)

target_link_libraries(GLFW_ContextAPI PRIVATE stb VenomCore glfw)

target_include_directories(GLFW_ContextAPI PUBLIC "./")

target_compile_definitions(GLFW_ContextAPI PRIVATE CONTEXT_EXPORTS)