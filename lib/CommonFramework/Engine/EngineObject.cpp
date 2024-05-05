#include <common/Engine/EngineObjectPool.h>

EngineObject::EngineObject()
{
    EngineObjectPool::InsertObject(this);
}

EngineObject::~EngineObject()
{
}

void EngineObject::DeleteObject()
{
    EngineObjectPool::DeleteObject(this);
}
