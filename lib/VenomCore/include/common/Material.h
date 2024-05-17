#pragma once

#include <common/DLL.h>
#include <common/Texture.h>

class GUI;

class Material : public VenomResource<Material>
{
public:
    ~Material();

    static Material* Create();
    Venom::ErrorCode ReloadObjectFromEngine() override;
    
    void SetAmbient(const glm::vec4 & ambient);
    void SetAmbient(Texture * ambientTexture);
    void SetDiffuse(const glm::vec4 & diffuse);
    void SetDiffuse(Texture * diffuseTexture);
    void SetSpecular(const glm::vec4 & specular);
    void SetSpecular(Texture * specularTexture);
    void SetBaseColor(const glm::vec4 & baseColor);
    void SetBaseColor(Texture * baseColorTexture);
    void SetNormal(Texture * normalTexture);
    void SetShininess(const float shininess);

    void UseAmbientTexture(bool use);
    void UseDiffuseTexture(bool use);
    void UseSpecularTexture(bool use);
    void UseNormalTexture(bool use);
    void UseBaseColorTexture(bool use);

    bool IsUsingAmbientTexture() const;
    bool IsUsingDiffuseTexture() const;
    bool IsUsingSpecularTexture() const;
    bool IsUsingNormalTexture() const;
    bool IsUsingBaseColorTexture() const;

    void AddTexture(Texture * texture);
    void SetTexture(int index, Texture* texture);
    const glm::vec4 & GetAmbient() const;
    const glm::vec4 & GetDiffuse() const;
    const glm::vec4 & GetSpecular() const;
    const glm::vec4 & GetBaseColor() const;
    const Texture * GetAmbientTexture() const;
    const Texture * GetDiffuseTexture() const;
    const Texture * GetSpecularTexture() const;
    const Texture * GetNormalTexture() const;
    const Texture * GetBaseColorTexture() const;
    const float GetShininess() const;
    const std::vector<Ptr<Texture>> & GetTextures() const;

protected:
    Material();

private:
    friend class GUI;
    
    glm::vec4 __ambient;
    Texture * __ambientTexture;
    
    glm::vec4 __diffuse;
    Texture * __diffuseTexture;
    
    glm::vec4 __specular;
    Texture * __specularTexture;
    
    glm::vec4 __baseColor;
    Texture * __baseColorTexture;
    
    Texture * __normalTexture;

    bool __useAmbientTexture;
    bool __useDiffuseTexture;
    bool __useSpecularTexture;
    bool __useNormalTexture;
    bool __useBaseColorTexture;

    float __shininess;
    std::vector<Ptr<Texture>> __textures;
};