#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoords;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

uniform vec3 cameraPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Transform vertex position to clip space
    gl_Position = projection * view * model * vec4(position, 1.0f);

    // Calculate fragment position in world space
    FragPos = vec3(model * vec4(position, 1.0));

    // Calculate normal in world space
    Normal = mat3(transpose(inverse(model))) * normal;

    // Calculate the direction from fragment to camera
    vec3 viewDir = normalize(cameraPos - FragPos);

    // Adjust the normal to face the camera
    Normal = normalize(Normal + viewDir);

    // Pass texture coordinates to fragment shader
    TexCoords = textureCoords;
}