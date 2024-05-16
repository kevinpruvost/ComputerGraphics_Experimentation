# CMakeList.txt : CMake project for VenomEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project (VenomCore)

# Set source files for the library
file(GLOB_RECURSE SOURCES "*.cpp" "*.hpp" "*.h")

add_library(VenomCore SHARED ${SOURCES})

target_link_libraries(VenomCore PUBLIC glm::glm spirv-cross-c-shared ryml::ryml c4core fmt::fmt imgui assimp::assimp)

target_include_directories(VenomCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(VenomCore PRIVATE VENOM_EXPORTS)
target_compile_features(VenomCore PUBLIC cxx_std_20)