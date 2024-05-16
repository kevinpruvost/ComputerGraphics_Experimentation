#pragma once

#include <common/ECS/Component.h>

class Renderer : public VenomComponent<Renderer>
{
public:
    Renderer();
    ~Renderer();

    void Init() override;
    void Update() override;
};