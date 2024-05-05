#pragma once

#include <common/ShaderPipeline.h>
#include <common/Texture.h>

class Skybox : public Drawable3D
{
public:
    Skybox(ShaderPipeline * shader, Texture * texture);
    void Draw() override;
    Venom::ErrorCode ReloadObjectFromEngine() override;

private:
    ShaderPipeline * __shader;
    Texture * __texture;

};