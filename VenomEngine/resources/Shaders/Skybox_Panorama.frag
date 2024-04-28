#version 330 core

in vec3 TexCoords;

out vec4 fragColor;

uniform sampler2D textureSampler;

void main()
{   
    // Sample the panorama texture using the spherical coordinates
    vec2 sphericalCoords = vec2(atan(TexCoords.x, TexCoords.z) / (2.0 * 3.14159265359) + 0.5, acos(TexCoords.y) / 3.14159265359);
    fragColor = texture(textureSampler, sphericalCoords);
}
