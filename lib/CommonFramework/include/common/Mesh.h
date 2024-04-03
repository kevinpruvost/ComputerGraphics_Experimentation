#pragma once

#include <common/Drawable3D.h>
#include <common/Face.h>
#include <common/DLL.h>

class Mesh : public Drawable3D
{
public:
    virtual ~Mesh() = default;

    COMMONFRAMEWORK_API static Mesh * CreateMesh();

    virtual const TriangleArray & GetTriangles() const = 0;
};