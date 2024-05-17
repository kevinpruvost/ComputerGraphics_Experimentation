#include <common/Engine/EngineObjectPool.h>

EngineObject::EngineObjectType EngineResource::ClassEngineObjectType = EngineObject::EngineObjectType::Resource;

EngineObject::EngineObject(const EngineObjectType type)
    : __engineObjectType{ type }
{
    EngineObjectPool::InsertObject(this);
}

EngineObject::~EngineObject()
{
}

void EngineObject::DeleteEngineObject()
{
    EngineObjectPool::DeleteObject(this);
}

EngineObject::EngineObjectType EngineObject::GetEngineObjectType() const
{
    return __engineObjectType;
}

static std::unordered_map<EngineResource::ResourceType, std::vector<EngineResource *>> s_resources;

const char* EngineResource::GetResourceName() const
{
    std::string oeuf;
    oeuf += _resourceName;
    return _resourceName.c_str();
}

const char* EngineResource::GetResourceNameRef()
{
    return _resourceName.c_str();
}

void EngineResource::SetResourceName(const char* name)
{
    _resourceName = name;
}

std::vector<EngineResource*>& EngineResource::GetResourcesOfSameType()
{
    return s_resources[__type];
}

std::vector<EngineResource*>& EngineResource::GetResourcesOfType(ResourceType type)
{
    return s_resources[type];
}

EngineResource::ResourceType EngineResource::GetResourceType() const
{
    return __type;
}

EngineResource::EngineResource(const ResourceType type)
    : EngineObject(EngineObjectType::Resource), _resourceName{}, __type(type)
{
    s_resources[type].push_back(this);
}

EngineResource::~EngineResource()
{
    const auto ite = std::find(s_resources[__type].begin(), s_resources[__type].end(), this);
    if (ite != s_resources[__type].end())
    {
        s_resources[__type].erase(ite);
    }
}
