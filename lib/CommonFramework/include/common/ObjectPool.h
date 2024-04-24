#pragma once

#include <unordered_map>
#include <common/Memory.h>

class Object
{
public:
    Object();
    ~Object();
    /**
     * @brief Deletes Object from ObjectPool and returns nullptr for assignment
     * @return nullptr
     */
    void * Delete();
};

class ObjectPool
{
public:
    ObjectPool();
    ~ObjectPool();

    static void Clear();
    static void AddObject(Object* object);
    static void RemoveObject(Object* object);

private:
    static std::unordered_map<Object *, SPtr<Object>> __objects;
};