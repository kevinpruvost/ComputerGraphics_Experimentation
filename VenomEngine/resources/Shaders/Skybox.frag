#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube textureSampler0;

void main() {
    FragColor = texture(textureSampler0, TexCoords);
}
