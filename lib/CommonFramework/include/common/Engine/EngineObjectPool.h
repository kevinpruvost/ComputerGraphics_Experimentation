#pragma once

#include <common/Engine/EngineObject.h>
#include <common/Common.h>

class EngineObjectPool
{
public:
    EngineObjectPool();
    ~EngineObjectPool();

    static void InsertObject(EngineObject* object);
    static void DeleteObject(EngineObject* object);
    static Venom::ErrorCode ReloadObjects();
};