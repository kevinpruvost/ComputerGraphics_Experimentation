#include <common/Components/Renderer.h>

#include <common/Rendering.h>

Renderer::Renderer()
    : VenomComponent<Renderer>()
    , __transform{ nullptr }
    , __model{ nullptr }
{
}

Renderer::~Renderer()
{
}

void Renderer::Init()
{
    __transform = _entity->AddComponent<Transform>();
    SerializeProperty("Model", &__model);
}

void Renderer::Update()
{
    if (__model == nullptr) return;
    Rendering::DrawModel(__model);
}
