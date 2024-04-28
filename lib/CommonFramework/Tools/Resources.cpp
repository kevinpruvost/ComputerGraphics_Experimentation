#include <common/Resources.h>

#include <unordered_map>

static std::unordered_map<std::string, SPtr<Resource>> resources;

void Resources::Clear()
{
    resources.clear();
}

Resource* Resources::GetResource(const char const * name)
{
    auto it = resources.find(name);
    if (it != resources.end())
        return it->second.get();
    return nullptr;
}

void Resources::AddResource(const char const * name, Resource* resource)
{
    resources[name] = SPtr<Resource>(resource);
}
