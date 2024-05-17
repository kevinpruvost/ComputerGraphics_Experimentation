#pragma once

#include <common/ECS/Component.h>
#include <common/Components/Transform.h>

class Light : public VenomComponent<Light>
{
public:
    Light();
    ~Light();

    void Init() override;

    // Updates the particle system based on the time passed
    void Update() override;

private:
    ColorRGB __ambient, __diffuse, __specular;
    float __intensity;
    Transform* __transform;
};