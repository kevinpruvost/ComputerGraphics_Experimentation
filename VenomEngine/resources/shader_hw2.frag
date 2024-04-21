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
    vec3 finalColor = useVertColor == 1 ? vertColor : texColor.rgb;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(1.0f, 1.0f, 1.0f)); // Example light direction
    
    FragColor = texColor;

//    // Check if the surface is facing towards the light source
//    if (dot(norm, lightDir) > 0.0) {
//        float ambientStrength = 0.2f;
//        vec3 ambient = ambientStrength * finalColor;
//
//        float diff = max(dot(norm, lightDir), 0.0);
//        vec3 diffuse = diff * finalColor;
//
//        vec3 result = (ambient + diffuse) * texColor.rgb;
//    
//        FragColor = vec4(result, texColor.a);
//    } else {
//        // If the surface is facing away from the light, output the texture color directly
//        FragColor = texColor;
//    }
}
