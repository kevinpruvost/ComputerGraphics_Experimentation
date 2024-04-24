#pragma once

#include <common/Vertex.h>
#include <common/Face.h>
#include <common/Common.h>
#include <common/ObjectPool.h>

class Drawable3D : public Object
{
public:
    enum DrawMode
    {
        POINTS    = 0b1,
        WIREFRAME = 0b10,
        SOLID     = 0b100,
        WIREFRAME_THROUGH = 0b1000, // No depth testing
        WIREFRAME_SOLID = WIREFRAME | SOLID,
        WIREFRAME_POINTS = WIREFRAME | POINTS,
        SOLID_POINTS = SOLID | POINTS,
        WIREFRAME_SOLID_POINTS = WIREFRAME | SOLID | POINTS
    };

    void SetDrawMode(DrawMode mode);
    virtual void Draw() = 0;
protected:
    Drawable3D();

    DrawMode _drawMode;
};

