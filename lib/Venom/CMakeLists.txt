# CMakeList.txt : CMake project for VenomEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project (Venom)

# Set source files for the library
file(GLOB_RECURSE SOURCES "*.cpp" "*.hpp" "*.h")

add_library(Venom SHARED ${SOURCES})

target_link_libraries(Venom PUBLIC glm::glm spirv-cross-c-shared ryml::ryml c4core fmt::fmt imgui assimp::assimp)

target_include_directories(Venom PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(Venom PRIVATE VENOM_EXPORTS)
target_compile_features(Venom PUBLIC cxx_std_20)