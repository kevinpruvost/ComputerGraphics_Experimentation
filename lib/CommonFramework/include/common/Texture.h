#pragma once

#include <common/DLL.h>
#include <common/Mesh.h>
#include <common/Material.h>
#include <common/Face.h>

class COMMONFRAMEWORK_API Texture
{
public:
    virtual ~Texture() = default;

    static Texture* CreateTexture();
    static inline Texture* CreateTexture(const std::filesystem::path& path)
    {
        Texture* texture = CreateTexture();
        texture->CreateFromFile(path);
        return texture;
    }

    virtual void CreateFromFile(const std::filesystem::path& path) = 0;
    virtual int GetTextureID() const = 0;
    virtual void BindTexture() const = 0;

protected:

protected:
    Texture() = default;

private:
};

