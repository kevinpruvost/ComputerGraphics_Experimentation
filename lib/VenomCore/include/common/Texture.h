#pragma once

#include <common/DLL.h>
#include <common/Face.h>
#include <common/Resources.h>

class VENOM_API Texture : public VenomResource<Texture>
{
public:
    virtual ~Texture() = default;

    static Texture* CreateTexture();

    virtual Venom::ErrorCode CreateFromFile(const std::filesystem::path& path) = 0;

    /**
     * @brief Creates cubemap from paths of faces, has to be from this order:
     * [right, left, top, bottom, back, front]
     * @param paths 
     * @return 
     */
    virtual Venom::ErrorCode CreateCubemap(const std::array<std::filesystem::path, 6>& paths) = 0;
    virtual int GetTextureID() const = 0;
    enum class TextureType : int
    {
        Texture1D,
        Texture2D,
        TextureCubemap
    };
    virtual void BindTexture(const TextureType textureType = TextureType::Texture2D, int textureID = 0) const = 0;

protected:

protected:
    Texture();

private:
};