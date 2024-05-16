#include <common/Systems/GeneralSystem.h>
#include <common/ECS/Component.h>

static GeneralSystem generalSystem;

Venom::ErrorCode GeneralSystem::Init(std::vector<Entity*>* entities)
{
    for (auto entity : *entities)
    {
        for (auto & pair : *entity->GetComponents())
        {
            auto component = pair.second;
            component->Init();
        }
    }
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode GeneralSystem::Update(std::vector<Entity*>* entities)
{
    for (auto entity : *entities)
    {
        for (auto & pair : *entity->GetComponents())
        {
            auto component = pair.second;
            component->Update();
        }
    }
    return Venom::ErrorCode::Success;
}
