#pragma once

#include <common/Math_Base.h>
#include <common/Engine/EngineObject.h>
#include <common/MemoryPool.h>
#include <common/Callback.h>
#include <string>

class Entity;
class Component;
typedef WPtr<Entity> EntityRef;

// Function to remove pointer
template<typename T>
using RemovePointer = typename std::remove_pointer<T>::type;

class COMMONFRAMEWORK_API PropertyManager {
public:
    struct Property
    {
        const char* name;

        enum class Type : uint8_t
        {
            FLOAT,
            INT,
            BOOL,
            STRING,
            VEC3,
            VEC4,
            MAT3,
            MAT4,
            ENGINE_OBJECT_PTR,
            OBJECT_PTR
        } type;

        union
        {
            float* f;
            int* i;
            bool* b;
            char* s;
            glm::vec3* vec3;
            glm::vec4* vec4;
            glm::mat3* mat3;
            glm::mat4* mat4;
            EngineObject** engineObject;
            Entity** object;
        };
    };

    std::vector<Property> _properties;
#ifdef _DEBUG
    std::unordered_map<const char*, Property> _propertiesNamesCheck;
#endif

public:
    std::vector<Property>& GetProperties() { return _properties; }

    template<typename T>
    void SetProperty(const char* name, T** prop) {
        Property p;
        p.name = name;
        if constexpr (std::is_base_of<EngineObject, T>::value) {
            p.type = Property::Type::ENGINE_OBJECT_PTR;
            p.engineObject = reinterpret_cast<EngineObject**>(prop);
        }
        else if constexpr (std::is_base_of<Entity, T>::value) {
            p.type = Property::Type::OBJECT_PTR;
            p.object = reinterpret_cast<Entity**>(prop);
        }
        else {
            assert(false && "Unsupported property type");
        }
        _properties.push_back(p);
#ifdef _DEBUG
        assert(_propertiesNamesCheck.find(name) == _propertiesNamesCheck.end() && "Can't add property with same name");
        _propertiesNamesCheck[name] = p;
#endif
    }

    template<typename T>
    void SetProperty(const char* name, T& prop) {
        Property p;
        p.name = name;
        // Determine the type and set the corresponding enum
        if constexpr (std::is_same<T, float>::value) {
            p.type = Property::Type::FLOAT;
            p.f = &prop;
        }
        else if constexpr (std::is_same<T, int>::value) {
            p.type = Property::Type::INT;
            p.i = &prop;
        }
        else if constexpr (std::is_same<T, bool>::value) {
            p.type = Property::Type::BOOL;
            p.b = &prop;
        }
        else if constexpr (std::is_same<T, char*>::value) {
            p.type = Property::Type::STRING;
            p.s = &prop;
        }
        else if constexpr (std::is_same<T, glm::vec3>::value) {
            p.type = Property::Type::VEC3;
            p.vec3 = &prop;
        }
        else if constexpr (std::is_same<T, glm::vec4>::value) {
            p.type = Property::Type::VEC4;
            p.vec4 = &prop;
        }
        else if constexpr (std::is_same<T, glm::mat3>::value) {
            p.type = Property::Type::MAT3;
            p.mat3 = &prop;
        }
        else if constexpr (std::is_same<T, glm::mat4>::value) {
            p.type = Property::Type::MAT4;
            p.mat4 = &prop;
        }
        else {
            assert(false && "Unsupported property type");
        }
        _properties.push_back(p);
#ifdef _DEBUG
        assert(_propertiesNamesCheck.find(name) == _propertiesNamesCheck.end() && "Can't add property with same name");
        _propertiesNamesCheck[name] = p;
#endif
    }
};

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
    std::vector<PropertyManager::Property>& GetProperties() { return _properties.GetProperties(); }
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
    inline void SerializeProperty(const char* name, T prop) {
        _properties.SetProperty(name, prop);
    }

    void SetGUICallback(CallbackContainer<void> callback);
    std::unordered_map<int, Component*>* GetComponents();
protected:
    Entity(const char* const name);
    friend class GUI;
    Callback<void>& GetGUICallback() { return _guiCallback; }
protected:
    PropertyManager _properties;
    std::string _objectName;
    Callback<void> _guiCallback;
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