#include <common/ECS/System.h>

System::System()
{
    SystemManager::AddSystem(this);
}

System::~System()
{
}

std::vector<System*> SystemManager::__systems;

SystemManager::SystemManager()
{
}

SystemManager::~SystemManager()
{
}

void SystemManager::AddSystem(System* object)
{
    __systems.push_back(object);
}

Venom::ErrorCode SystemManager::Init(std::vector<Entity*>* entities)
{
    for (auto system : __systems)
    {
        if (system->Init(entities) != Venom::ErrorCode::Success)
        {
            return Venom::ErrorCode::Failure;
        }
    }
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode SystemManager::Update(std::vector<Entity*>* entities)
{
    for (auto system : __systems)
    {
        if (system->Update(entities) != Venom::ErrorCode::Success)
        {
            return Venom::ErrorCode::Failure;
        }
    }
    return Venom::ErrorCode::Success;
}
