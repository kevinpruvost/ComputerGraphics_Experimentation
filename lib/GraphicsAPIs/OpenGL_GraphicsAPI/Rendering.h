#pragma once

#include <common/Rendering.h>
#include <common/Vertex.h>
#include <glad/glad.h>


class Rendering_OGL : public Rendering
{
public:
    void _SetDepthTest(bool enable) const override;
    void _SetBlending(bool enable) const override;
    void _SetBlendingFunction(BlendingFunction src, BlendingFunction dst) const override;
    void _SetBlendingEquation(BlendingEquation eq) const override;
    void _DrawVertices(const VertexBuffer * vertices) const override;
    void _SetDrawMode(const Drawable3D::DrawMode drawMode) const;
    void _DrawModel(const Model* model) const override;

    void _ClearDepthBuffer() const override;
    void _ClearStencilBuffer() const override;
    void _ClearColorBuffer(const glm::vec4& color) const override;
};

extern "C"
{
    EXPORT Rendering* DLL_SINGLETON_LOADING_NAME(Rendering)();
}