#pragma once

#include <common/Math_Base.h>
#include <common/Engine/EngineObject.h>
#include <common/MemoryPool.h>
#include <common/Callback.h>
#include <common/Interfaces/IScene.h>
#include <string>

class Entity;
class Component;
typedef WPtr<Entity> EntityRef;

class VENOM_API Entity : public MemoryPoolObject
{
public:
    static Entity * CreateEntity(const char* const name = nullptr);
    ~Entity();
    /**
     * @brief Deletes Object from ObjectPool and returns nullptr for assignment
     * @return nullptr
     */
    void* Delete();

    template<typename T>
    inline void SetProperty(const char* name, T& prop)
    {
        return _properties.SetProperty(name, prop);
    }
    const char* GetObjectName() const { return _objectName.c_str(); }
    void SetEntityName(const char* name) { _objectName = name; }

    template<typename T>
    T * AddComponent() {
        {
            auto ite = __components.find(VenomComponent<T>::ID);
            if (ite != __components.end()) {
                return reinterpret_cast<T *>(ite->second);
            };
        }
        T* component = Component::CreateComponent<T>();
        component->SetEntity(this);
        if (Scene::IsStarted())
            component->Init();
        __components[VenomComponent<T>::ID] = component;
        return component;
    }
    template<typename T>
    void RemoveComponent() {
        auto ite = __components.find(VenomComponent<T>::ID);
        if (ite != __components.end()) {
            T* component = reinterpret_cast<T*>(ite->second);
            __components.erase(ite);
            delete component;
        }
    }
    template<typename T>
    bool HasComponent() const {
        return __components.find(VenomComponent<T>::ID) != __components.end();
    }

    std::unordered_map<int, Component*>* GetComponents();
protected:
    Entity(const char* const name);
    friend class GUI;
    Component* AddComponent(int componentID);
    void RemoveComponent(int componentID);
    bool HasComponent(int componentID) const;
protected:
    std::string _objectName;
private:
    std::unordered_map<int, Component *> __components;
};

#include <common/Memory.h>

class VENOM_API EntityPool
{
public:
    EntityPool();
    ~EntityPool();

    static void Clear();
    static void AddEntity(Entity* object);
    static void DeleteEntity(Entity* object);
    static std::vector<Entity*> * GetAllEntities();

private:
    static std::vector<Entity*> __entities;
};