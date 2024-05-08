#pragma once

#include <common/Common.h>
#include <common/MemoryPool.h>

class EngineObject : virtual public MemoryPoolObject
{
public:
    virtual ~EngineObject();

    void DeleteEngineObject();
    virtual Venom::ErrorCode ReloadObjectFromEngine() = 0;

    enum class EngineObjectType : uint8_t
    {
        Object,
        Resource
    };

    EngineObjectType GetEngineObjectType() const;
protected:
    EngineObject(const EngineObjectType type);

private:
    const EngineObjectType __engineObjectType;
};

class EngineResource : public EngineObject
{
public:
    enum class ResourceType : uint8_t
    {
        TEXTURE,
        MATERIAL,
        SHADER,
        MODEL,
        SOUND,
        FONT,
        MESH
    };

    const char* GetResourceName() const;
    const char* GetResourceNameRef();
    void SetResourceName(const char* name);

    std::vector<EngineResource*>& GetResourcesOfSameType();

    EngineResource(const ResourceType type);
    virtual ~EngineResource();
protected:
    std::string _resourceName;
private:
    const ResourceType __type;
};