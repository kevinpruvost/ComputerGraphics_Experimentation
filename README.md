# Computer Graphics Experimentation

All of the code that is present here has been entirely written and implemented by myself alone.

This repository is dedicated to the coursework for "Fundamentals of Computer Graphics" at Tsinghua University during the Spring semester of 2024.

The projects undertaken in this course will cover various subjects, including:

- Mesh Subdivision & Simplification
- Ray Tracing
- Human Skin Rendering

These projects will be implemented using OpenGL, Vulkan, and DirectX11 at minimum, with the possibility of incorporating DirectX12 if time permits.

## Project Structure

The project is currently a work in progress (WIP) and is organized as follows:

- **Context API DLLs:** This project compiles Dynamic Link Libraries (DLLs) related to context APIs, such as GLFW or Win32 native Windows API.

- **Graphics API DLLs:** DLLs related to graphics APIs, including OpenGL, Vulkan, DirectX11, and potentially other APIs in the future.

- **GUI API DLLs:** This section contains the DLLs related to graphical user interface (GUI) APIs. These libraries facilitate the creation and management of graphical user interfaces within the application. Especially useful as ImGUI implementation differs considering the Context and Graphics API used.

- **External Libraries DLLs:** For external libraries used in the project. These libraries are necessary dependencies for the project's functionality and may include components related to graphics rendering, image loading, YAML parsing, mathematics operations, and other functionalities required by the application.

- **Main Application:** The Main Application section is the core of the project, encompassing the main executable or application files.

Further documentation will be released in the near ending phase of the project.

## External Libraries

The following external libraries have been brought into the project:

- **[glad](https://github.com/Dav1dde/glad):** An OpenGL loader generated by the glad tool.

- **[glfw](https://github.com/glfw/glfw):** A multi-platform library for creating windows, contexts, and handling input.

- **[stb_image](https://github.com/nothings/stb):** A single-header library for loading images in various formats.

- **[yaml-cpp](https://github.com/jbeder/yaml-cpp):** A YAML parser and emitter in C++.

- **[glm](https://github.com/g-truc/glm):** A header-only C++ mathematics library for graphics software based on the OpenGL Shading Language (GLSL) specifications.

- **[SPIRV-Cross](https://github.com/KhronosGroup/SPIRV-Cross):** A tool to convert SPIR-V into other forms like GLSL, HLSL, and MSL.

- **[Dear ImGui](https://github.com/ocornut/imgui):** A bloat-free graphical user interface library for C++ with minimal dependencies.

Feel free to explore these repositories for more information on each library.

## Useful Links

- **[Interactive ImGUI Online Manual](https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html)**

- **[John Carmack notes](https://cppdepend.com/blog/john-carmack-cpp-programming-legend/)**

## Notes on Good Practices

- Avoid `throw` and `exception`, they put too much overhead as they are managed by external instances and might actually be unsafe in contrary to managing errors with `return` codes.

- Avoid templated code as it might be recompiled for nothing, thus taking size on `dll` and `exe` files but also making each DLL has its own version of the function (and different function adresses), might be very problematic.
