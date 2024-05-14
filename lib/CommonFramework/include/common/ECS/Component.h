#pragma once

class Entity;
class Component
{
public:
    virtual ~Component() = default;
    Component(const Component&) = delete;

    virtual void Update() = 0;
    virtual void Init() = 0;

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
    static int getComponentID() {
        return IDCounter++;
    }

private:
    friend class Entity;
    template<typename T>
    static T* CreateComponent() {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
        return new T();
    }
};

template<typename T>
class VenomComponent : public Component
{
public:
    static const int ID;
};

template<typename T>
const int VenomComponent<T>::ID = Component::getComponentID();