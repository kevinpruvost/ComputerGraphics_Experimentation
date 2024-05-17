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

    const glm::vec3 & GetPosition() const;
    const ColorRGB & GetColor() const;
    float GetIntensity() const;
    float GetAttenuation() const;
    void SetColor(const ColorRGB& color);
    void SetIntensity(float intensity);
    void SetAttenuation(float attenuation);

    static const std::vector<Light *> * GetLights();

private:
    ColorRGB __color;
    float __intensity;
    float __attenuation;
    Transform* __transform;
};