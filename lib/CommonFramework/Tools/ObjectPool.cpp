#include <common/ObjectPool.h>

constexpr const int batch_size = 200;

std::unordered_map<Object*, SPtr<Object>> ObjectPool::__objects;

Object::Object()
{
    ObjectPool::AddObject(this);
}

Object::~Object()
{
}

void * Object::Delete()
{
    ObjectPool::RemoveObject(this);
    return nullptr;
}

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
}

void ObjectPool::AddObject(Object* object)
{
    __objects[object] = SPtr<Object>(object);
    if (__objects.size() % batch_size == 0)
        __objects.reserve(__objects.size() + batch_size);
}

void ObjectPool::RemoveObject(Object* object)
{
    __objects.erase(object);
}
