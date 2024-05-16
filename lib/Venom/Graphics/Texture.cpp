#include <common/Texture.h>

#include <common/Engine/EngineLoader.h>
#include <algorithm>

typedef Texture * (*CreateTextureFn)();

Texture* Texture::CreateTexture()
{
    CreateTextureFn createTextureFn = EngineLoader::GetEngineDll()->getFunction<CreateTextureFn>("createTexture");
    assert(createTextureFn != nullptr);
    Texture * texture = createTextureFn();
    assert(texture != nullptr);
    return texture;
}

Texture::Texture()
    : Resource(EngineResource::ResourceType::TEXTURE)
{
}

Texture::TextureType StringToTextureType(String str)
{
    str.ToLower();
    if (str == "1d")
    {
        return Texture::TextureType::Texture1D;
    }
    else if (str == "cubemap")
    {
        return Texture::TextureType::TextureCubemap;
    }
    else // Default if nullptr or not specified
    {
        return Texture::TextureType::Texture2D;
    }
    return Texture::TextureType::Texture2D;
}

template<>
Texture * Resources::_Load(const char const* name, const YamlNode & data)
{
    Logger::DebugPrint("Loading texture: %s", name);

    Texture * texture = Texture::CreateTexture();
    Texture::TextureType type = Texture::TextureType::Texture2D;
    if (data.has_child("type"))
        type = StringToTextureType(YamlNodeToString(data["type"]));
    const auto & pathNode = data["path"];
    switch (type)
    {
        case Texture::TextureType::Texture1D:
        case Texture::TextureType::Texture2D:
            texture->CreateFromFile(YamlNodeToPath(pathNode));
            break;
        case Texture::TextureType::TextureCubemap:
        {
                std::array<std::filesystem::path, 6> paths;
                for (int i = 0; i < 6; ++i)
                {
                    paths[i] = (YamlNodeToPath(pathNode[i]));
                }
                texture->CreateCubemap(paths);
        }
            break;
    }
    return texture;
}