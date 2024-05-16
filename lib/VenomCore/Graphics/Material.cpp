#include <common/Material.h>

Material::Material()
    : Resource(EngineResource::ResourceType::MATERIAL)
{
}

Material::~Material()
{

}

Material* Material::Create()
{
    return new Material();
}

Venom::ErrorCode Material::ReloadObjectFromEngine()
{
    return Venom::ErrorCode::Success;
}

void Material::SetAmbient(const glm::vec4& ambient)
{
    __ambient = ambient;
}

void Material::SetDiffuse(const glm::vec4& diffuse)
{
    __diffuse = diffuse;
}

void Material::SetSpecular(const glm::vec4& specular)
{
    __specular = specular;
}

void Material::SetShininess(const float shininess)
{
    __shininess = shininess;
}

void Material::AddTexture(Texture* texture)
{
    __textures.push_back(texture);
}

const glm::vec4& Material::GetAmbient() const
{
    return __ambient;
}

const glm::vec4& Material::GetDiffuse() const
{
    return __diffuse;
}

const glm::vec4& Material::GetSpecular() const
{
    return __specular;
}

const float Material::GetShininess() const
{
    return __shininess;
}

const std::vector<Ptr<Texture>>& Material::GetTextures() const
{
    return __textures;
}
