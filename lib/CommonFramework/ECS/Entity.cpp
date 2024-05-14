#include <common/ECS/Entity.h>

#include <random>
#include <cstring>

constexpr const int batch_size = 200;

std::vector<Entity*> EntityPool::__entities;
static std::unordered_set<Entity*> entitiesToRemove;
static EntityPool entityPool;

static const char* GenerateObjectName() {
    // Define a character set for random string generation
    constexpr char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz123456789";
    constexpr int length = 16;

    // Generate a random string
    static thread_local std::random_device rd;
    static thread_local std::mt19937 gen(rd());
    static thread_local std::uniform_int_distribution<int> dist(0, sizeof(charset) - 2); // -2 to avoid including the string terminator

    static thread_local char randomString[length + 1]; // +1 for the null terminator
    for (int i = 0; i < length; ++i) {
        randomString[i] = charset[dist(gen)];
    }
    randomString[length] = 0;

    // Construct the object name
    static char objectName[length + 8] = "Object_\0"; // +8 for "Object_" prefix and null terminator
    memcpy(objectName + (sizeof(char) * 7), randomString, sizeof(char) * length);

    // Return the object name as const char*
    return objectName;
}

Entity::Entity(const char const* name)
    : MemoryPoolObject()
    , _objectName{ name ? name : GenerateObjectName() }
    , _guiCallback(nullptr)
{
    EntityPool::AddEntity(this);
}

Entity * Entity::CreateEntity(const char* const name)
{
    Entity * entity = new Entity(name);
    return entity;
}

Entity::~Entity()
{
}

void* Entity::Delete()
{
    EntityPool::DeleteEntity(this);
    return nullptr;
}

#pragma region Pool

EntityPool::EntityPool()
{
    __entities.reserve(batch_size);
}

EntityPool::~EntityPool()
{
    Clear();
}

void EntityPool::Clear()
{
    __entities.clear();
}

void EntityPool::AddEntity(Entity* object)
{
    __entities.push_back(object);
}

void EntityPool::DeleteEntity(Entity* object)
{
    entitiesToRemove.insert(object);
}

std::vector<Entity*>& EntityPool::GetAllEntities()
{
    // If entities to remove, then remove
    for (int i = 0; entitiesToRemove.size() > 0 && i < __entities.size(); ++i)
    {
        auto ite = entitiesToRemove.find(__entities[i]);
        if (ite != entitiesToRemove.end()) {
            MemoryPool::DeleteObject(__entities[i]);
            __entities.erase(__entities.begin() + i);
            entitiesToRemove.erase(ite);
        }
    }
    return __entities;
}

#pragma endregion