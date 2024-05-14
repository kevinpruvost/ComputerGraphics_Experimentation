# CMakeList.txt : CMake project for VenomEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project("VenomEngine")

set(CMAKE_DEBUG_POSTFIX "")

set(PROJECT_PATH ${CMAKE_SOURCE_DIR}/)
set(LIBS_PATH ${CMAKE_SOURCE_DIR}/lib/)
set(GRAPHICS_DLL_PROPS_PATH ${CMAKE_SOURCE_DIR}/GraphicsDLLProps.cmake)

set(CMAKE_CXX_STANDARD 20)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} "/O3")

# Libraries
add_subdirectory(${LIBS_PATH})

# Executable
add_subdirectory(${PROJECT_PATH}/VenomEngine)