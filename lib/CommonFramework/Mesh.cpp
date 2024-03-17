#include <common/Mesh.h>
#include <common/FrameworkLoader.h>

typedef Mesh * (*CreateMeshFn)();

Mesh* Mesh::CreateMesh()
{
    CreateMeshFn createMeshFn = FrameworkLoader::EngineDll->getFunction<CreateMeshFn>("createMesh");
    if (createMeshFn == nullptr)
        throw DLLException("Failed to load createMesh function from engine dll");
    Mesh* mesh = createMeshFn();
    if (mesh == nullptr)
        throw DLLException("Failed to create mesh");
    return mesh;
}
