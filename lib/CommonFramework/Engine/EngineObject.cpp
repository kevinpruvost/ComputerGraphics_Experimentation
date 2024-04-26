#include <common/Engine/EngineObjectPool.h>

EngineObject::EngineObject()
{
}

EngineObject::~EngineObject()
{
}

void EngineObject::DeleteObject()
{
    EngineObjectPool::DeleteObject(this);
}
