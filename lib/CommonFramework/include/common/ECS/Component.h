#pragma once

class Entity;
class Component
{
public:
    virtual ~Component() = default;
    Component(const Component&) = delete;

    virtual void Update() = 0;
    virtual void Init() = 0;

    template<typename T>
    T* CreateComponent() {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
        return new T();
    }

protected:
    friend class Entity;
    void SetEntity(Entity* entity) {
        _entity = entity;
    }

    Component() = default;
    Entity* _entity;
};