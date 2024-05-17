#pragma once

#include <common/ECS/Entity.h>

class GUI;

#pragma region PropertyManager

struct EngineObjectContainer
{
    EngineObject** obj;
    EngineObject::EngineObjectType type;
    EngineResource::ResourceType resourceType;
};

struct ColorRGBA : public glm::vec4
{
    using glm::vec4::vec4;
    operator glm::vec4() { return *this; }
};

struct ColorRGB : public glm::vec3
{
    using glm::vec3::vec3;
    operator glm::vec3() { return *this; }
};

class VENOM_API PropertyManager {
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
            COLOR_RGB,
            COLOR_RGBA,
            MAT3,
            MAT4,
            ENGINE_OBJECT_PTR,
            ENTITY_PTR,
            COMPONENT_PTR
        } type;

        union
        {
            float* f;
            int* i;
            bool* b;
            char* s;
            glm::vec3* vec3;
            glm::vec4* vec4;
            ColorRGB* colorRGB;
            ColorRGBA* colorRGBA;
            glm::mat3* mat3;
            glm::mat4* mat4;
            EngineObjectContainer engineObject;
            Entity** entity;
            Component** component;
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
            p.engineObject.obj = reinterpret_cast<EngineObject**>(prop);
            p.engineObject.type = T::ClassEngineObjectType;
            p.engineObject.resourceType = T::GetClassResourceType();
        }
        else if constexpr (std::is_base_of<Entity, T>::value) {
            p.type = Property::Type::ENTITY_PTR;
            p.entity = reinterpret_cast<Entity**>(prop);
        }
        else if constexpr (std::is_base_of<Component, T>::value) {
            p.type = Property::Type::COMPONENT_PTR;
            p.component = reinterpret_cast<Component**>(prop);
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
        else if constexpr (std::is_same<T, ColorRGB>::value) {
            p.type = Property::Type::COLOR_RGB;
            p.colorRGB = &prop;
        }
        else if constexpr (std::is_same<T, ColorRGBA>::value) {
            p.type = Property::Type::COLOR_RGBA;
            p.colorRGBA = &prop;
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

#pragma endregion

class VENOM_API Component
{
public:
    virtual ~Component() = default;
    Component(const Component&) = delete;

    virtual void Update() = 0;
    virtual void Init() = 0;

    void SetEntityName(const char* name);
    int GetComponentID();
    const char * GetComponentName();
    static int WatchID() { return IDCounter; }

protected:
    friend class Entity;
    friend class GUI;

    Callback<void, GUI *>& GetGUICallback();
    Callback<void, GUI *> _guiCallback;

    void SetEntity(Entity* entity) {
        _entity = entity;
    }

    Component(int componentID);
    Entity* _entity;

    // Static member variable to hold component ID
    static int IDCounter;
    int _componentID;
    // Function to get component ID and store polymorphic constructor
    static int GetNewComponentID(const char* componentName, Component *(*constructor)());
    static std::vector<const char*>* GetAllComponentNames();
    static Component* CreateComponent(int componentID);
protected:
    std::vector<PropertyManager::Property>& GetProperties();
    template<typename T>
    inline void SerializeProperty(const char* name, T & prop) { _properties.SetProperty(name, prop); }
    template<typename T>
    inline void SerializeProperty(const char* name, T ** prop) { _properties.SetProperty(name, prop); }
    PropertyManager _properties;


private:
    friend class Entity;
    template<typename T>
    static T* CreateComponent() {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
        return new T();
    }
};

template<typename T>
class VENOM_API VenomComponent : public Component
{
public:
    VenomComponent()
        : Component(ID)
    {}

    static const int ID;
    static const char * Name;
};

template<typename T>
const int VenomComponent<T>::ID = Component::GetNewComponentID(typeid(T).name(), []() -> Component* { return new T(); });

template<typename T>
const char* VenomComponent<T>::Name = typeid(T).name();