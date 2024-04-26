#include <common/Mesh.h>
#include <common/Engine/EngineLoader.h>

typedef Mesh* (*CreateMeshFn)();

Mesh* Mesh::CreateMesh()
{
    CreateMeshFn createMeshFn = EngineLoader::GetEngineDll()->getFunction<CreateMeshFn>("createMesh");
    if (createMeshFn == nullptr)
        throw DLLException("Failed to load createMesh function from engine dll");
    Mesh* mesh = createMeshFn();
    if (mesh == nullptr)
        throw DLLException("Failed to create mesh");
    return mesh;
}