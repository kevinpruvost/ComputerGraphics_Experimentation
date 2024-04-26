#version 330 core
in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D textureSampler;
uniform vec3 vertColor;
uniform int useVertColor;

void main()
{
    vec4 texColor = texture(textureSampler, TexCoords);
    vec4 finalColor = useVertColor == 1 ? vec4(vertColor, 1.0) : texColor;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(1.0f, 1.0f, 1.0f)); // Example light direction
    
    FragColor = finalColor;
}
