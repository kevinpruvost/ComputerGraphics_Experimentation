#version 330 core
in vec2 TexCoords;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D textureSampler;
uniform vec4 color;

void main()
{
    vec4 texColor = texture(textureSampler, TexCoords);
    texColor = vec4(texColor.r, texColor.g, texColor.b, 1.0f - texColor.r);
    vec4 finalColor = mix(texColor, texColor * color, color.a);
    finalColor.a *= color.a;

    FragColor = finalColor;
}
