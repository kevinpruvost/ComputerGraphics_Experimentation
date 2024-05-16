#pragma once

#include <common/Texture.h>
#include <glad/glad.h>

class Texture_OGL : public Texture
{
public:
    Texture_OGL();
    ~Texture_OGL();

    Venom::ErrorCode CreateFromFile(const std::filesystem::path& path) override;
    Venom::ErrorCode CreateCubemap(const std::array<std::filesystem::path, 6>& paths) override;
    int GetTextureID() const override;
    void BindTexture(const TextureType textureType, int textureID) const override;
    Venom::ErrorCode ReloadObjectFromEngine() override;

private:
    Venom::ErrorCode GenerateTexture();

private:
    GLuint __textureID;
};

extern "C"
{
    EXPORT Texture * createTexture();
}