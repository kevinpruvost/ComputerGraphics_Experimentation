#include <common/Drawable3D.h>

void Drawable3D::SetDrawMode(DrawMode mode)
{
    _drawMode = mode;
}

Drawable3D::Drawable3D()
    : _drawMode(DrawMode::SOLID)
{
}
