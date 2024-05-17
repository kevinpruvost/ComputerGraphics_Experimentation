#include <common/Material.h>

DECLARE_VENOM_RESOURCE_STATIC(Material, MATERIAL);
Material::Material()
    : VenomResource<Material>()
    , __ambient(glm::vec4(0.0f))
    , __diffuse(glm::vec4(0.0f))
    , __specular(glm::vec4(0.0f))
    , __baseColor(glm::vec4(0.0f))
    , __ambientTexture(nullptr)
    , __diffuseTexture(nullptr)
    , __specularTexture(nullptr)
    , __baseColorTexture(nullptr)
    , __shininess(0.0f)
    , __normalTexture(nullptr)
    , __useAmbientTexture(false)
    , __useDiffuseTexture(false)
    , __useSpecularTexture(false)
    , __useNormalTexture(false)
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
    __useAmbientTexture = false;
}

void Material::SetAmbient(Texture* ambientTexture)
{
    __ambientTexture = ambientTexture;
    __useAmbientTexture = ambientTexture != nullptr;
}

void Material::SetDiffuse(const glm::vec4& diffuse)
{
    __diffuse = diffuse;
    __useDiffuseTexture = false;
}

void Material::SetDiffuse(Texture* diffuseTexture)
{
    __diffuseTexture = diffuseTexture;
    __useDiffuseTexture = diffuseTexture != nullptr;
}

void Material::SetSpecular(const glm::vec4& specular)
{
    __specular = specular;
    __useSpecularTexture = false;
}

void Material::SetSpecular(Texture* specularTexture)
{
    __specularTexture = specularTexture;
    __useSpecularTexture = specularTexture != nullptr;
}

void Material::SetBaseColor(const glm::vec4& baseColor)
{
    __baseColor = baseColor;
    __useBaseColorTexture = false;
}

void Material::SetBaseColor(Texture* baseColorTexture)
{
    __baseColorTexture = baseColorTexture;
    __useBaseColorTexture = baseColorTexture != nullptr;
}

void Material::SetNormal(Texture* normalTexture)
{
    __normalTexture = normalTexture;
    __useNormalTexture = normalTexture != nullptr;
}

void Material::SetShininess(const float shininess)
{
    __shininess = shininess;
}

void Material::UseAmbientTexture(bool use)
{
    __useAmbientTexture = use;
}

void Material::UseDiffuseTexture(bool use)
{
    __useDiffuseTexture = use;
}

void Material::UseSpecularTexture(bool use)
{
    __useSpecularTexture = use;
}

void Material::UseNormalTexture(bool use)
{
    __useNormalTexture = use;
}

void Material::UseBaseColorTexture(bool use)
{
    __useBaseColorTexture = use;
}

bool Material::IsUsingAmbientTexture() const
{
    return __useAmbientTexture;
}

bool Material::IsUsingDiffuseTexture() const
{
    return __useDiffuseTexture;
}

bool Material::IsUsingSpecularTexture() const
{
    return __useSpecularTexture;
}

bool Material::IsUsingNormalTexture() const
{
    return __useNormalTexture;
}

bool Material::IsUsingBaseColorTexture() const
{
    return __useBaseColorTexture;
}

void Material::AddTexture(Texture* texture)
{
    __textures.push_back(texture);
}

void Material::SetTexture(int index, Texture* texture)
{
    if (index < __textures.size())
    {
        __textures[index] = texture;
    }
    else
    {
        __textures.push_back(texture);
    }
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

const glm::vec4& Material::GetBaseColor() const
{
    return __baseColor;
}

const Texture* Material::GetAmbientTexture() const
{
    return __ambientTexture;
}

const Texture* Material::GetDiffuseTexture() const
{
    return __diffuseTexture;
}

const Texture* Material::GetSpecularTexture() const
{
    return __specularTexture;
}

const Texture* Material::GetNormalTexture() const
{
    return __normalTexture;
}

const Texture* Material::GetBaseColorTexture() const
{
    return __baseColorTexture;
}

const float Material::GetShininess() const
{
    return __shininess;
}

const std::vector<Ptr<Texture>>& Material::GetTextures() const
{
    return __textures;
}
