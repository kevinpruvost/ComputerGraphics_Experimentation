#pragma once

#include <common/Components/Drawable3D.h>
#include <common/Face.h>
#include <common/DLL.h>
#include <common/Resources.h>
#include <common/Vertex.h>
#include <common/Material.h>

class Mesh : public Resource
{
public:
    virtual ~Mesh() = default;

    COMMONFRAMEWORK_API static Mesh * CreateMesh();
    void SetVertexBuffer(VertexBuffer * vertexBuffer);
    const VertexBuffer * GetVertexBuffer() const;
    void SetMaterialId(int materialId);
    const int GetMaterialId() const;
protected:
    Mesh();

    VertexBuffer * _vertexBuffer;
    int _materialId;
};