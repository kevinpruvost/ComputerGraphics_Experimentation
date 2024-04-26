#include <common/Engine/EngineObjectPool.h>
#include <common/Memory.h>
#include <unordered_map>

static std::unordered_map<EngineObject*, SPtr<EngineObject>> engineObjects;
static EngineObjectPool engineObjectPool;
static int batch_size = 2;

EngineObjectPool::EngineObjectPool()
{
    engineObjects.reserve(batch_size);
}

EngineObjectPool::~EngineObjectPool()
{
    engineObjects.clear();
}

void EngineObjectPool::InsertObject(EngineObject* object)
{
    engineObjects[object] = SPtr<EngineObject>(object);
    if (engineObjects.size() >= batch_size)
    {
        engineObjects.reserve(engineObjects.size() + batch_size);
    }
}

void EngineObjectPool::DeleteObject(EngineObject* object)
{
    std::unordered_map<EngineObject *, SPtr<EngineObject>>::iterator ite = engineObjects.find(object);
    if (ite != engineObjects.end())
        engineObjects.erase(ite);
}

Venom::ErrorCode EngineObjectPool::ReloadObjects()
{
    for (std::unordered_map<EngineObject*, SPtr<EngineObject>>::iterator ite = engineObjects.begin(); ite != engineObjects.end(); ++ite)
    {
        Venom::ErrorCode result = ite->second->ReloadObjectFromEngine();
        if (result != Venom::ErrorCode::Success)
            return result;
    }
    return Venom::ErrorCode::Success;
}
