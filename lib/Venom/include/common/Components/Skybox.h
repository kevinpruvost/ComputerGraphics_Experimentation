#pragma once

#include <common/ShaderPipeline.h>
#include <common/Texture.h>
#include <common/ECS/Component.h>

class Skybox : public VenomComponent<Skybox>
{
public:
    Skybox();
    void Draw() const;
    void Update() override;
    void Init() override;

    ShaderPipeline * shader;
    Texture * texture;
};