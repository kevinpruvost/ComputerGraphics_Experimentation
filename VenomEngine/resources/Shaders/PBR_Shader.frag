#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D ambientMap;
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D normalMap;
    sampler2D baseColorMap;
    vec4 ambientColor;
    vec4 diffuseColor;
    vec4 specularColor;
    vec4 baseColor;
    bool useAmbientMap;
    bool useDiffuseMap;
    bool useSpecularMap;
    bool useNormalMap;
    bool useBaseColorMap;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
    float attenuation;  // Single attenuation factor
};

in vec2 TexCoords;
in vec3 FragPos;  
in vec3 Normal;  
in vec3 Tangent;
in vec3 Bitangent;

uniform vec3 cameraPos;
uniform Material material;
uniform Light light[1];

void main()
{
    // Determine the normal
    vec3 actualNormal;
    if (material.useNormalMap) {
        // Obtain normal from normal map in tangent space
        vec3 tangentNormal = texture(material.normalMap, TexCoords).rgb;
        tangentNormal = tangentNormal * 2.0 - 1.0; // Transform from [0, 1] range to [-1, 1]

        // Construct TBN matrix
        vec3 T = normalize(Tangent);
        vec3 B = normalize(Bitangent);
        vec3 N = normalize(Normal);
        mat3 TBN = mat3(T, B, N);

        // Transform normal from tangent space to world space
        actualNormal = normalize(TBN * tangentNormal);
    } else {
        actualNormal = normalize(Normal);
    }

    // Sample textures or use default values
    vec4 ambientColor = material.useAmbientMap ? texture(material.ambientMap, TexCoords) : material.ambientColor;
    vec4 diffuseColor = material.useDiffuseMap ? texture(material.diffuseMap, TexCoords) : material.diffuseColor;
    vec4 specularColor = material.useSpecularMap ? texture(material.specularMap, TexCoords) : material.specularColor;
    vec4 baseColor = material.useBaseColorMap ? texture(material.baseColorMap, TexCoords) : material.baseColor;

    // Light calculations
    vec3 lightDir = normalize(light[0].position - FragPos);
    float diff = max(dot(actualNormal, lightDir), 0.0);

    // Diffuse component
    vec4 diffuse = diff * diffuseColor * baseColor * vec4(light[0].color, 1.0) * light[0].intensity;

    // Specular component
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, actualNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec4 specular = spec * specularColor * vec4(light[0].color, 1.0) * light[0].intensity;

    // Ambient component
    vec4 ambient = ambientColor * baseColor * vec4(light[0].color, 1.0) * light[0].intensity;

    // Attenuation
    float distance = length(light[0].position - FragPos);
    float attenuation = 1.0 / (1.0 + light[0].attenuation * distance * distance);

    // Apply attenuation
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // Combine results
    vec4 result = ambient + diffuse + specular;
    result.a = baseColor.a; // Assuming base color alpha is used for transparency
    FragColor = result;
}
