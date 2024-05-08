#include <common/Resources.h>

#include <unordered_map>

static std::unordered_map<std::string, Ptr<Resource>> resources;

void Resources::Clear()
{
    resources.clear();
}

Resource* Resources::GetResource(const char const * name)
{
    auto it = resources.find(name);
    if (it != resources.end())
        return it->second;
    return nullptr;
}

Venom::ErrorCode Resources::AddResource(const char const * name, Resource* resource)
{
    if (resources.find(name) != resources.end())
    {
        Logger::Print("Resource already exists: %s", name);
        return Venom::ErrorCode::Failure;
    }
    resource->SetResourceName(name);
    resources[name] = resource;
    return Venom::ErrorCode::Success;
}

Resource::Resource(const ResourceType type)
    : EngineResource(type)
{
}
