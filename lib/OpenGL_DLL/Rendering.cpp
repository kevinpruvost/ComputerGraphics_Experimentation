#include "Rendering.h"

EXPORT Rendering* DLL_SINGLETON_LOADING_NAME(Rendering)()
{
    return new Rendering_OGL();
}

void Rendering_OGL::_SetDepthTest(bool enable) const
{
    if (enable)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

static GLenum GetGLBlendingFunction(Rendering::BlendingFunction func)
{
    switch (func)
    {
    case Rendering::BlendingFunction::ONE:
        return GL_ONE;
    case Rendering::BlendingFunction::ONE_MINUS_SRC_ALPHA:
        return GL_ONE_MINUS_SRC_ALPHA;
    case Rendering::BlendingFunction::SRC_ALPHA:
        return GL_SRC_ALPHA;
    case Rendering::BlendingFunction::ONE_MINUS_SRC_COLOR:
        return GL_ONE_MINUS_SRC_COLOR;
    }
}

void Rendering_OGL::_SetBlendingFunction(BlendingFunction src, BlendingFunction dst) const
{
    glBlendFunc(GetGLBlendingFunction(src), GetGLBlendingFunction(dst));
}

static GLenum GetGLBlendingEquation(Rendering::BlendingEquation eq)
{
    switch (eq)
    {
    case Rendering::BlendingEquation::Add:
        return GL_FUNC_ADD;
    case Rendering::BlendingEquation::Subtract:
        return GL_FUNC_SUBTRACT;
    }
}

void Rendering_OGL::_SetBlendingEquation(BlendingEquation eq) const
{
    glBlendEquation(GetGLBlendingEquation(eq));
}

void Rendering_OGL::_DrawVertices(const VertexBuffer * vertices) const
{
    vertices->Bind();
    glDrawArrays(GL_TRIANGLES, 0, vertices->GetVertexCount());
    vertices->Unbind();
}

void Rendering_OGL::_SetDrawMode(const Drawable3D::DrawMode drawMode) const
{
    switch (drawMode)
    {
    case Drawable3D::DrawMode::SOLID:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    case Drawable3D::DrawMode::WIREFRAME:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case Drawable3D::DrawMode::POINTS:
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        break;
    default:
        assert(false && "Can only do SOLID, WIREFRAME or POINTS");
        break;
    }
}


