#pragma once

#include <common/Vertex.h>
#include <common/Face.h>
#include <common/Common.h>
#include <common/Engine/EngineObject.h>

class Drawable3D : public EngineObject
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
        WIREFRAME_SOLID_POINTS = WIREFRAME | SOLID | POINTS,
        NONE = 0
    };
    static DrawMode GetDrawMode(bool points, bool wireframe, bool solid);

    void SetDrawMode(DrawMode mode);
    virtual void Draw() = 0;
protected:
    Drawable3D();

    DrawMode _drawMode;
};

