#pragma once

#include <common/ECS/Component.h>
#include <common/Components/Transform.h>

class Renderer : public VenomComponent<Renderer>
{
public:
    Renderer();
    ~Renderer();

    void Init() override;
    void Update() override;

private:
    Transform* __transform;
    Model* __model;
};