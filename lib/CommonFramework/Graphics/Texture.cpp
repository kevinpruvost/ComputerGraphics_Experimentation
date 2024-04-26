#include <common/Texture.h>

#include <common/Engine/EngineLoader.h>

typedef Texture * (*CreateTextureFn)();

Texture* Texture::CreateTexture()
{
    CreateTextureFn createTextureFn = EngineLoader::GetEngineDll()->getFunction<CreateTextureFn>("createTexture");
    if (createTextureFn == nullptr)
        throw DLLException("Failed to load createTexture function from engine dll");
    Texture * texture = createTextureFn();
    if (texture == nullptr)
        throw DLLException("Failed to create texture");
    return texture;
}