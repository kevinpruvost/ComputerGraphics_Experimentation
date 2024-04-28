#version 330 core

in vec2 texCoord; // Input texture coordinates from vertex shader
out vec4 fragColor; // Output fragment color

uniform sampler2D textureSampler; // Texture sampler uniform

void main()
{
    fragColor = texture(textureSampler, texCoord); // Sample texture using texture coordinates
}
