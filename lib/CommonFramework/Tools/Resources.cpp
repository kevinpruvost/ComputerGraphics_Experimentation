#include <common/Resources.h>

#include <unordered_map>

static std::unordered_map<std::string, SPtr<Resource>> resources;

void Resources::Clear()
{
    resources.clear();
}

Resource* Resources::GetResource(const char* name)
{
    auto it = resources.find(name);
    if (it != resources.end())
        return it->second.get();
    return nullptr;
}

void Resources::AddResource(const char* name, Resource* resource)
{
    resources[name] = SPtr<Resource>(resource);
}
