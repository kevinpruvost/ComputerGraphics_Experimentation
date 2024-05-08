#pragma once

#include <common/Math_Base.h>
#include <common/Engine/EngineObject.h>
#include <common/MemoryPool.h>
#include <common/Callback.h>
#include <string>

class Object;

// Function to remove pointer
template<typename T>
using RemovePointer = typename std::remove_pointer<T>::type;

class PropertyManager {
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
            float * f;
            int * i;
            bool * b;
            char * s;
            glm::vec3 * vec3;
            glm::vec4 * vec4;
            glm::mat3 * mat3;
            glm::mat4 * mat4;
            EngineObject ** engineObject;
            Object ** object;
        };
    };

    std::vector<Property> _properties;
#ifdef _DEBUG
    std::unordered_map<const char*, Property> _propertiesNamesCheck;
#endif

public:
    std::vector<Property> & GetProperties() { return _properties; }

    template<typename T>
    void SetProperty(const char* name, T ** prop) {
        Property p;
        p.name = name;
        if constexpr (std::is_base_of<EngineObject, T>::value) {
            p.type = Property::Type::ENGINE_OBJECT_PTR;
            p.engineObject = reinterpret_cast<EngineObject**>(prop);
        }
        else if constexpr (std::is_base_of<Object, T>::value) {
            p.type = Property::Type::OBJECT_PTR;
            p.object = reinterpret_cast<Object**>(prop);
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

class Object : virtual public MemoryPoolObject
{
public:
    Object(const char * const name = nullptr);
    ~Object();
    /**
     * @brief Deletes Object from ObjectPool and returns nullptr for assignment
     * @return nullptr
     */
    void* Delete();

    template<typename T>
    inline void SetProperty(const char* name, T & prop)
    {
        return _properties.SetProperty(name, prop);
    }
    std::vector<PropertyManager::Property> & GetProperties() { return _properties.GetProperties(); }
    const char* GetObjectName() { return _objectName.c_str(); }
    void SetObjectName(const char * name) { _objectName = name; }
    void SetGUICallback(CallbackContainer<void> callback) { _guiCallback = callback; }
    Callback<void> & GetGUICallback() { return _guiCallback; }
protected:
    PropertyManager _properties;
    std::string _objectName;
    Callback<void> _guiCallback;
};
