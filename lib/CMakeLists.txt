# CMakeList.txt : CMake project for VenomEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project("Libraries")

set(PROJECT_PATH ${CMAKE_SOURCE_DIR}/)
set(LIBS_PATH ${CMAKE_SOURCE_DIR}/lib/)

set(CMAKE_CXX_STANDARD 20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(BUILD_SHARED_LIBS ON)
set(SPIRV_CROSS_SHARED ON)

# External Libraries
add_subdirectory("./fmt")
set_target_properties(fmt
PROPERTIES FOLDER "ExternalLibraries")

add_subdirectory("./glad")

add_subdirectory("./glfw")
set_target_properties(
    glfw
PROPERTIES FOLDER "ExternalLibraries")

add_subdirectory("./imgui")

add_subdirectory("./SPIRV-CROSS")
set_target_properties(
    spirv-cross spirv-cross-c spirv-cross-core spirv-cross-glsl spirv-cross-hlsl spirv-cross-msl
    spirv-cross-reflect spirv-cross-util spirv-cross-cpp
    spirv-cross-c-shared
PROPERTIES FOLDER "ExternalLibraries/SPIRV-CROSS")

add_subdirectory("./stb")

add_subdirectory("./rapidyaml")
set_target_properties(
    ryml ryml-uninstall
    c4core-amalgamate c4core
PROPERTIES FOLDER "ExternalLibraries")

add_subdirectory("./glm")
set_target_properties(
    glm
PROPERTIES FOLDER "ExternalLibraries")
find_package(glm CONFIG REQUIRED)

# Common Framework Library
add_subdirectory("./CommonFramework")

# Graphics API Libraries
add_subdirectory("./Vulkan_DLL")
add_subdirectory("./OpenGL_DLL")
add_subdirectory("./DX12_DLL")

# GUI Libraries
add_subdirectory("./GUI")

# Context Libraries
add_subdirectory("./GLFWWindow")
add_subdirectory("./WinNativeWindow")