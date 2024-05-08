#include "Texture.h"
#include "Mesh.h"
#include "BufferManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

// Export the factory function to create an instance of the class
EXPORT Texture * createTexture() {
    return new Texture_OGL();
}

Texture_OGL::Texture_OGL()
    : Texture()
    , __textureID{ 0 }
{
}

Texture_OGL::~Texture_OGL()
{
    if (__textureID != 0) {
        glDeleteTextures(1, &__textureID);
    }
}

Venom::ErrorCode Texture_OGL::CreateFromFile(const std::filesystem::path& path)
{
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        GenerateTexture();
        BindTexture(TextureType::Texture2D, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        Logger::Print("Failed to load texture: %s", path.string().c_str());
        return Venom::ErrorCode::Failure;
    }
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode Texture_OGL::CreateCubemap(const std::array<std::filesystem::path, 6>& paths)
{
    GenerateTexture();
    BindTexture(TextureType::TextureCubemap, 0);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < paths.size(); i++)
    {
        unsigned char* data = stbi_load(paths[i].string().c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            Logger::Print("Failed to load cubemap texture: %s", paths[i].string().c_str());
            return Venom::ErrorCode::Failure;
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return Venom::ErrorCode::Success;
}

int Texture_OGL::GetTextureID() const
{
    return __textureID;
}

void Texture_OGL::BindTexture(const TextureType textureType, int textureID) const
{
    glActiveTexture(GL_TEXTURE0 + textureID);
    GLenum textureTypeGL;
    switch (textureType)
    {
        case TextureType::Texture1D:
            textureTypeGL = GL_TEXTURE_1D;
            break;
        case TextureType::Texture2D:
            textureTypeGL = GL_TEXTURE_2D;
            break;
        case TextureType::TextureCubemap:
            textureTypeGL = GL_TEXTURE_CUBE_MAP;
            break;
    };
    glBindTexture(textureTypeGL, __textureID);
}

Venom::ErrorCode Texture_OGL::ReloadObjectFromEngine()
{
    Logger::Print("Not implemented");
    return Venom::ErrorCode::Failure;
}

Venom::ErrorCode Texture_OGL::GenerateTexture()
{
    glGenTextures(1, &__textureID);
    return Venom::ErrorCode::Success;
}
