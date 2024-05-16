# CMakeList.txt : CMake project for VenomEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project (WinNative_ContextAPI)

# Set source files for the library
file(GLOB SOURCES "*.cpp" "*.h")

add_library(WinNative_ContextAPI SHARED ${SOURCES})

set_target_properties(WinNative_ContextAPI
    PROPERTIES FOLDER "ContextAPIs"
)

target_link_libraries(WinNative_ContextAPI PRIVATE VenomCore)

target_compile_definitions(WinNative_ContextAPI PRIVATE CONTEXT_EXPORTS)