#include <common/Texture.h>

#include <common/Engine/EngineLoader.h>

typedef Texture * (*CreateTextureFn)();

Texture* Texture::CreateTexture()
{
    CreateTextureFn createTextureFn = EngineLoader::GetEngineDll()->getFunction<CreateTextureFn>("createTexture");
    assert(createTextureFn != nullptr);
    Texture * texture = createTextureFn();
    assert(texture != nullptr);
    return texture;
}