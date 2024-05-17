#include <common/Components/Light.h>

static UPtr<std::vector<Light*>> lights;

Light::Light()
    : __intensity{ 1.0f }
    , __color{ ColorRGB(1.0f) }
    , __attenuation{ 1.0f }
{
    if (lights.get() == nullptr)
    {
        lights = new std::vector<Light*>();
        lights->reserve(32);
    }
    lights->push_back(this);
}

Light::~Light()
{
    auto it = std::find(lights->begin(), lights->end(), this);
    if (it != lights->end())
        lights->erase(it);
}

void Light::Init()
{
    __transform = _entity->AddComponent<Transform>();
    SerializeProperty("Color", __color);
    SerializeProperty("Intensity", __intensity);
    SerializeProperty("Attenuation", __attenuation);
}

void Light::Update()
{
}

const glm::vec3& Light::GetPosition() const
{
    return __transform->GetPosition();
}

float Light::GetAttenuation() const
{
    return __attenuation;
}

void Light::SetColor(const ColorRGB& color)
{
    __color = color;
}

void Light::SetIntensity(float intensity)
{
    __intensity = intensity;
}

void Light::SetAttenuation(float attenuation)
{
    __attenuation = attenuation;
}

const std::vector<Light*> * Light::GetLights()
{
    return lights.get();
}

float Light::GetIntensity() const
{
    return __intensity;
}

const ColorRGB & Light::GetColor() const
{
    return __color;
}