#pragma once

#include <common/DLL.h>
#include <common/ECS/Entity.h>

template<class SystemType>
class VenomSystem;

class COMMONFRAMEWORK_API System
{
public:
    ~System();

protected:
    System();

public:
    virtual Venom::ErrorCode Init(std::vector<Entity*>* entities) = 0;
    virtual Venom::ErrorCode Update(std::vector<Entity*> * entities) = 0;
};

template<class SystemType>
class VenomSystem : public System
{
public:
    ~VenomSystem() = default;

protected:
    VenomSystem() = default;
};

class COMMONFRAMEWORK_API SystemManager
{
public:
    ~SystemManager();

    static void AddSystem(System* object);
    static Venom::ErrorCode Init(std::vector<Entity*>* entities);
    static Venom::ErrorCode Update(std::vector<Entity*> * entities);

private:
    SystemManager();
    static std::vector<System*> __systems;
};