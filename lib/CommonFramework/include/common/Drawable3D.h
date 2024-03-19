#pragma once

#include <common/Vertex.h>
#include <common/Common.h>

class Drawable3D
{
public:
    virtual void SetVertices(const VertexArray& vertices) = 0;
    virtual void Draw() = 0;
};

