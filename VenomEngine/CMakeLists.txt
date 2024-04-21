# CMakeList.txt : CMake project for VenomEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("VenomEngineExe")

# Add source to this project's executable.
add_executable (VenomEngineExe
    "main.cpp"
    "Config.yaml"
)

target_link_libraries(VenomEngineExe CommonFramework imgui glm::glm)

# Copy resources directory to the build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/ DESTINATION ${CMAKE_BINARY_DIR}/resources/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Config.yaml DESTINATION ${CMAKE_BINARY_DIR})

message("Copying resources dir: " ${CMAKE_SOURCE_DIR}/resources/" to "${CMAKE_BINARY_DIR})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VenomEngineExe PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
