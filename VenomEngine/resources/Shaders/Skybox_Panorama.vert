#version 330 core

layout(location = 0) in vec3 vertexPosition;

out vec3 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = vertexPosition; // Pass texture coordinates as vertex position
    gl_Position = projection * view * vec4(vertexPosition, 1.0);
}
