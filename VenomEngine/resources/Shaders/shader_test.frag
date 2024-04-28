#version 330 core
in vec3 ourColor;

uniform vec3 vertColor;
uniform int useVertColor;

out vec4 color;

void main()
{
    color = vec4(useVertColor == 1 ? vertColor : ourColor, 1.0f);
}