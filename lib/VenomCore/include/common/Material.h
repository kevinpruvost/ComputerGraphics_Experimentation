#pragma once

#include <common/DLL.h>
#include <common/Texture.h>

class Material : public Resource
{
public:
    ~Material();

    static Material* Create();
    Venom::ErrorCode ReloadObjectFromEngine() override;
    void SetAmbient(const glm::vec4 & ambient);
    void SetDiffuse(const glm::vec4 & diffuse);
    void SetSpecular(const glm::vec4 & specular);
    void SetShininess(const float shininess);
    void AddTexture(Texture * texture);
    const glm::vec4 & GetAmbient() const;
    const glm::vec4 & GetDiffuse() const;
    const glm::vec4 & GetSpecular() const;
    const float GetShininess() const;
    const std::vector<Ptr<Texture>> & GetTextures() const;

protected:
    Material();

private:
    glm::vec4 __ambient, __diffuse, __specular;
    float __shininess;
    std::vector<Ptr<Texture>> __textures;
};