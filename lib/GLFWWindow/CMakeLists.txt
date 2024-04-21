# CMakeList.txt : CMake project for VenomEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project (GLFWWindow)

# Set source files for the library
file(GLOB SOURCES "*.cpp" "*.h")

add_library(GLFWWindow SHARED ${SOURCES})

set_target_properties(GLFWWindow
    PROPERTIES FOLDER "ContextAPIs"
)

target_link_libraries(GLFWWindow PRIVATE stb CommonFramework glfw)

target_include_directories(GLFWWindow PUBLIC "./")

target_compile_definitions(GLFWWindow PRIVATE CONTEXT_EXPORTS)