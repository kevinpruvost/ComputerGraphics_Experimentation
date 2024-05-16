#pragma once

#include <unordered_map>
#include <common/Memory.h>

class MemoryPoolObject
{
public:
    MemoryPoolObject();
    virtual ~MemoryPoolObject();
protected:
    template <typename T>
    inline WPtr<T> GetRef() { return WPtr<T>(__selfRef); }
private:
    WPtr<MemoryPoolObject> __selfRef;
};

class MemoryPool
{
public:
    MemoryPool();
    ~MemoryPool();

    static void Clear();
    static void DeleteObject(MemoryPoolObject* object);
    static WPtr<MemoryPoolObject> AddObject(MemoryPoolObject * object);
private:
    static std::unordered_map<MemoryPoolObject*, SPtr<MemoryPoolObject>> __objects;
};