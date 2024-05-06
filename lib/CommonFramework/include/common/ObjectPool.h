#pragma once

#include <common/Object.h>

#include <unordered_map>
#include <common/Memory.h>

class ObjectPool
{
public:
    ObjectPool();
    ~ObjectPool();

    static void Clear();
    static void AddObject(Object* object);
    static void DeleteObject(Object* object);
    static std::vector<Object *> & GetAllObjects();

private:
    static std::unordered_set<Object *> __objects;
};