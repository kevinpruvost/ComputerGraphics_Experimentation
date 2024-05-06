#include <common/ObjectPool.h>

#include <common/MemoryPool.h>

constexpr const int batch_size = 200;

std::unordered_set<Object*> ObjectPool::__objects;
static std::vector<Object*> allObjects;
static bool allObjectsDirty = true;
static ObjectPool objectPool;

ObjectPool::ObjectPool()
{
    __objects.reserve(batch_size);
}

ObjectPool::~ObjectPool()
{
    Clear();
}

void ObjectPool::Clear()
{
    __objects.clear();
    allObjects.clear();
}

void ObjectPool::AddObject(Object* object)
{
    __objects.insert(object);
    if (__objects.size() % batch_size == 0)
    {
        __objects.reserve(__objects.size() + batch_size);
    }
    allObjectsDirty = true;
}

void ObjectPool::DeleteObject(Object* object)
{
    assert(__objects.find(object) != __objects.end() && "Object not found in pool");
    __objects.erase(object);
    allObjectsDirty = true;
    MemoryPool::DeleteObject(object);
}

std::vector<Object *> & ObjectPool::GetAllObjects()
{
    if (allObjectsDirty)
    {
        allObjects.clear();
        allObjects.reserve(__objects.size());
        allObjects.insert(allObjects.end(), __objects.begin(), __objects.end());
        allObjectsDirty = false;
    }
    return allObjects;
}
