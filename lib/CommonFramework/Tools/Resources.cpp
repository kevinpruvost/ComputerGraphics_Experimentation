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

void Resources::AddResource(const char const * name, Resource* resource)
{
    resource->SetResourceName(name);
    resources[name] = resource;
}

Resource::Resource(const ResourceType type)
    : EngineResource(type)
{
}
