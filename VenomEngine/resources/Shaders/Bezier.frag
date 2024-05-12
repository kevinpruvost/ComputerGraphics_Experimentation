#version 410 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D textureSampler0;

void main()
{
    color = texture(textureSampler0, TexCoord);
    //color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
