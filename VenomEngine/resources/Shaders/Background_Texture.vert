#version 330 core

layout(location = 0) in vec2 position; // Vertex position attribute
out vec2 texCoord; // Output texture coordinates to fragment shader

void main()
{
    gl_Position = vec4(position, 0.0, 1.0); // Set vertex position
    texCoord = position * 0.5 + 0.5; // Convert vertex position to texture coordinates (assuming quad covers [-1, 1] range)
}