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
    : __textureID{ 0 }
{
}

Texture_OGL::~Texture_OGL()
{
    if (__textureID != 0) {
        glDeleteTextures(1, &__textureID);
    }
}

void Texture_OGL::CreateFromFile(const std::filesystem::path& path)
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

        glGenTextures(1, &__textureID);
        glBindTexture(GL_TEXTURE_2D, __textureID);
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
        throw RuntimeException("Failed to load texture: {}", path.string().c_str());
    }
}

int Texture_OGL::GetTextureID() const
{
    return __textureID;
}

void Texture_OGL::BindTexture() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, __textureID);
}
