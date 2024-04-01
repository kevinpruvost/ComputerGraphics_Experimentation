#pragma once

#include <common/Drawable3D.h>
#include <common/Face.h>

class Mesh : public Drawable3D
{
public:
    virtual ~Mesh() = default;

    static Mesh * CreateMesh();

    virtual const TriangleArray & GetTriangles() const = 0;
};