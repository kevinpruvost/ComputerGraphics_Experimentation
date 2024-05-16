#include <common/Components/Drawable3D.h>

Drawable3D::DrawMode Drawable3D::GetDrawModeFromStates(bool points, bool wireframe, bool solid)
{
    DrawMode mode = DrawMode::NONE;
    if (points)
        mode = static_cast<DrawMode>(mode | DrawMode::POINTS);
    if (wireframe)
        mode = static_cast<DrawMode>(mode | DrawMode::WIREFRAME);
    if (solid)
        mode = static_cast<DrawMode>(mode | DrawMode::SOLID);
    return mode;
}

Drawable3D::DrawMode Drawable3D::GetDrawMode() const
{
    return _drawMode;
}

void Drawable3D::SetDrawMode(DrawMode mode)
{
    _drawMode = mode;
}

Drawable3D::Drawable3D()
    : _drawMode(DrawMode::GLOBAL)
{
}
