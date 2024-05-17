#pragma once

#include <common/Memory.h>
#include <common/Engine/EngineObject.h>
#include <type_traits>
#include <common/Yaml.h>

class VENOM_API Resource : public EngineResource
{
public:
    virtual ~Resource() = default;

protected:
    Resource(const ResourceType type);
};

template<class T>
class VENOM_API VenomResource : public Resource
{
public:
    VenomResource() : Resource(GetClassResourceType()) {}

    static ResourceType GetClassResourceType();
};
#define DECLARE_VENOM_RESOURCE_STATIC(Class, Resource) template<> EngineResource::ResourceType VenomResource<Class>::GetClassResourceType() { return EngineResource::ResourceType::Resource; }

class VENOM_API Resources
{
public:
    template <typename T>
    static T* Load(const char const * name, const YamlNode & data)
    {
        static_assert(std::is_base_of<Resource, T>::value, "T must be a derived class of Resource");
        Resource* resource = GetResource(name);
        if (resource == nullptr || dynamic_cast<T*>(resource) == nullptr)
        {
            resource = _Load<T>(name, data);
            if (AddResource(name, resource) != Venom::ErrorCode::Success)
                return nullptr;
        }
        return dynamic_cast<T*>(resource);
    }

    template <typename T>
    static T* Load(const char const* name)
    {
        static_assert(std::is_base_of<Resource, T>::value, "T must be a derived class of Resource");
        Resource* resource = GetResource(name);
        if (resource == nullptr)
        {
            Logger::Print("Resource not found: %s", name);
        }
        return dynamic_cast<T*>(resource);
    }

    template<typename T>
    static T* Create(const char const* name)
    {
        static_assert(std::is_base_of<Resource, T>::value, "T must be a derived class of Resource");
        Resource* resource = GetResource(name);
        if (resource != nullptr)
        {
            Logger::Print("Resource already exists: %s", name);
            return dynamic_cast<T*>(resource);
        }
        resource = T::Create();
        if (AddResource(name, resource) != Venom::ErrorCode::Success)
            return nullptr;
        return dynamic_cast<T*>(resource);
    }

    static void Clear();
    static Venom::ErrorCode AddResource(const char const* name, Resource* resource);

private:
    template <typename T>
    static T* _Load(const char const * name, const YamlNode & data);

    static Resource* GetResource(const char const * name);
};