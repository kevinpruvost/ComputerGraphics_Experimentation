#pragma once

#include <common/Memory.h>
#include <common/Object.h>
#include <type_traits>
#include <common/Yaml.h>

class Resource : public EngineResource
{
public:
    virtual ~Resource() = default;
protected:
    Resource(const ResourceType type);
};

class Resources
{
public:
    template <typename T>
    static T* Load(const char const * name, const YamlNode & data)
    {
        static_assert(std::is_base_of<Resource, T>::value, "T must be a derived class of Resource");
        Resource* resource = GetResource(name);
        if (resource == nullptr)
        {
            resource = _Load<T>(name, data);
            AddResource(name, resource);
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

    static void Clear();

private:
    template <typename T>
    static T* _Load(const char const * name, const YamlNode & data);

    static Resource* GetResource(const char const * name);
    static void AddResource(const char const * name, Resource* resource);
};