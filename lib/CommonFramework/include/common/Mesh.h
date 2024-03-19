#pragma once

#include <common/DLL.h>
#include <common/Drawable3D.h>

class Mesh : public Drawable3D
{
public:
    virtual ~Mesh() = default;

    //void SetIndices(unsigned int* indices, int size);
    //void SetTextureCoords(float* textureCoords, int size);
    //void SetNormals(float* normals, int size);
    static Mesh * CreateMesh();

protected:
    Mesh() = default;
};

