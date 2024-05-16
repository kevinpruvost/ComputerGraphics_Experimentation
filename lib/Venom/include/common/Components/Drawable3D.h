#pragma once

#include <common/Vertex.h>
#include <common/Face.h>
#include <common/Common.h>
#include <common/Engine/EngineObject.h>
#include <common/Components/Drawable.h>

class Drawable3D : public Drawable
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
        GLOBAL = 0b10000, // Takes global value as ref
        NONE = 0
    };
    static DrawMode GetDrawModeFromStates(bool points, bool wireframe, bool solid);

    DrawMode GetDrawMode() const;
    void SetDrawMode(DrawMode mode);
protected:
    Drawable3D();

    DrawMode _drawMode;
};

