#pragma once

#include <common/Texture.h>
#include <glad/glad.h>

class Texture_OGL : public Texture
{
public:
    Texture_OGL();
    ~Texture_OGL();

    void CreateFromFile(const std::filesystem::path& path) override;
    int GetTextureID() const override;
    void BindTexture() const override;

private:

private:
    GLuint __textureID;
};

extern "C"
{
    EXPORT Texture * createTexture();
}