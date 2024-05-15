#pragma once

#include <common/ECS/Entity.h>

class COMMONFRAMEWORK_API Component
{
public:
    virtual ~Component() = default;
    Component(const Component&) = delete;

    virtual void Update() = 0;
    virtual void Init() = 0;

    void SetEntityName(const char * name) {
        _entity->SetEntityName(name);
    }

protected:
    friend class Entity;
    void SetEntity(Entity* entity) {
        _entity = entity;
    }

    Component() = default;
    Entity* _entity;

    // Static member variable to hold component ID
    static int IDCounter;
    // Function to get component ID
    static int GetNewComponentID() { return IDCounter++; }

private:
    friend class Entity;
    template<typename T>
    static T* CreateComponent() {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
        return new T();
    }
};

template<typename T>
class COMMONFRAMEWORK_API VenomComponent : public Component
{
public:
    static const int ID;
    static const char * Name;
};

template<typename T>
const int VenomComponent<T>::ID = Component::GetNewComponentID();

template<typename T>
const char* VenomComponent<T>::Name = typeid(T).name();