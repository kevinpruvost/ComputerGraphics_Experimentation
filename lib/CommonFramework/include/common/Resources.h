#pragma once

#include <common/Memory.h>
#include <type_traits>

class Resource
{
public:
    virtual ~Resource() = default;
};

class Resources
{
public:
    template <typename T>
    static T* Load(const char* name)
    {
        static_assert(std::is_base_of<Resource, T>::value, "T must be a derived class of Resource");
        Resource* resource = GetResource(name);
        if (resource == nullptr)
        {
            resource = _Load<T>(name);
            AddResource(name, resource);
        }
        return dynamic_cast<T*>(resource);
    }

    static void Clear();

private:
    template <typename T>
    static T* _Load(const char* path);

    static Resource* GetResource(const char* name);
    static void AddResource(const char* name, Resource* resource);
};