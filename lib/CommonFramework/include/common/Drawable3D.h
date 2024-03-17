#pragma once

#include <common/Vector.h>
#include <common/Common.h>

class Drawable3D
{
public:
    virtual void SetVertices(const Vec3Array& vertices) = 0;
    virtual void Draw() = 0;
};

