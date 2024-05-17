#include <common/Components/Light.h>

Light::Light()
    : __intensity{ 1.0f }
    , __ambient{ ColorRGB(1.0f) }
    , __diffuse{ ColorRGB(1.0f) }
    , __specular{ ColorRGB(1.0f) }
{
}

Light::~Light()
{

}

void Light::Init()
{
    __transform = _entity->AddComponent<Transform>();
    SerializeProperty("Ambient", __ambient);
    SerializeProperty("Diffuse", __diffuse);
    SerializeProperty("Specular", __specular);
    SerializeProperty("Intensity", __intensity);
}

void Light::Update()
{
}
