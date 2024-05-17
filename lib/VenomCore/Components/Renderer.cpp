#include <common/Components/Renderer.h>

#include <common/Rendering.h>

Renderer::Renderer()
    : VenomComponent<Renderer>()
    , __transform{ nullptr }
    , __model{ nullptr }
    , __shader{ nullptr }
{
}

Renderer::~Renderer()
{
}

void Renderer::Init()
{
    __transform = _entity->AddComponent<Transform>();
    SerializeProperty("Model", &__model);
    SerializeProperty("Shader", &__shader);
}

void Renderer::Update()
{
    if (__model == nullptr || __shader == nullptr) return;
    __shader->Use();
    __shader->SetModelMatrix(__transform->GetModelMatrix());
    Rendering::GiveMainCameraProperties(__shader);
    Rendering::RenderLights();
    Rendering::DrawModel(__model);
}

void Renderer::SetModel(Model* model)
{
    __model = model;
}

void Renderer::SetShaderPipeline(ShaderPipeline* shaderPipeline)
{
    __shader = shaderPipeline;
}
