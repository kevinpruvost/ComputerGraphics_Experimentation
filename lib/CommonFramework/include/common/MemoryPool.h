#pragma once

#include <unordered_map>
#include <common/Memory.h>

class MemoryPoolObject
{
public:
    MemoryPoolObject();
    virtual ~MemoryPoolObject();
};

class MemoryPool
{
public:
    MemoryPool();
    ~MemoryPool();

    static void Clear();
    static void DeleteObject(MemoryPoolObject* object);
    static void AddObject(MemoryPoolObject * object);
private:
    static std::unordered_map<MemoryPoolObject*, SPtr<MemoryPoolObject>> __objects;
};