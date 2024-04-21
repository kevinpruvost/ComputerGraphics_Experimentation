# CMakeList.txt : CMake project for VenomEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project (WinNativeWindow)

# Set source files for the library
file(GLOB SOURCES "*.cpp" "*.h")

add_library(WinNativeWindow SHARED ${SOURCES})

set_target_properties(WinNativeWindow
    PROPERTIES FOLDER "ContextAPIs"
)

target_link_libraries(WinNativeWindow PRIVATE CommonFramework)

target_compile_definitions(WinNativeWindow PRIVATE CONTEXT_EXPORTS)