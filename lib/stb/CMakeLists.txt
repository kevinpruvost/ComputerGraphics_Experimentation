# CMakeList.txt : CMake project for VenomEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project (stb)

# Set source files for the library
file(GLOB SOURCES "*.c" "*.h" "*.hpp")

add_library(stb SHARED ${SOURCES})

target_include_directories(stb PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(stb
    PROPERTIES FOLDER "ExternalLibraries"
)