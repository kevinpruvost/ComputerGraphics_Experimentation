#pragma once

#include <common/Math_Base.h>
#include <common/Engine/EngineObject.h>
#include <common/MemoryPool.h>
#include <common/Callback.h>
#include <string>

class Entity;
class Component;
typedef WPtr<Entity> EntityRef;

class COMMONFRAMEWORK_API Entity : public MemoryPoolObject
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

    std::unordered_map<int, Component*>* GetComponents();
protected:
    Entity(const char* const name);
    friend class GUI;
protected:
    std::string _objectName;
private:
    std::unordered_map<int, Component *> __components;
};

#include <common/Memory.h>

class COMMONFRAMEWORK_API EntityPool
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