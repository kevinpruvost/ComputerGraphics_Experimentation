#include <common/MemoryPool.h>

constexpr const int batch_size = 200;

std::unordered_map<MemoryPoolObject*, SPtr<MemoryPoolObject>> MemoryPool::__objects;
static MemoryPool memoryPool;

MemoryPool::MemoryPool()
{
    __objects.reserve(batch_size);
}

MemoryPool::~MemoryPool()
{
    Clear();
}

void MemoryPool::Clear()
{
    __objects.clear();
}

void MemoryPool::DeleteObject(MemoryPoolObject* object)
{
    if (__objects.find(object) == __objects.end()) return;
    __objects.erase(object);
}

void MemoryPool::AddObject(MemoryPoolObject* object)
{
    __objects[object] = SPtr<MemoryPoolObject>(object);
    if (__objects.size() % batch_size == 0)
    {
        __objects.reserve(__objects.size() + batch_size);
    }
}

MemoryPoolObject::MemoryPoolObject()
{
    MemoryPool::AddObject(this);
}

MemoryPoolObject::~MemoryPoolObject()
{
}
