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

void Mesh::SetVertexBuffer(VertexBuffer* vertexBuffer)
{
    _vertexBuffer = vertexBuffer;
}

void Mesh::SetMaterialId(int materialId)
{
    _materialId = materialId;
}

const VertexBuffer* Mesh::GetVertexBuffer() const
{
    return _vertexBuffer;
}

const int Mesh::GetMaterialId() const
{
    return _materialId;
}

DECLARE_VENOM_RESOURCE_STATIC(Mesh, MESH);
Mesh::Mesh()
    : VenomResource<Mesh>()
    , _vertexBuffer{ nullptr }
    , _materialId{ 0 }
{

}