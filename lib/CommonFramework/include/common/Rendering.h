#pragma once

#include <common/DLL.h>
#include <common/Memory.h>
#include <common/Vertex.h>
#include <common/Model.h>
#include <common/Components/Drawable3D.h>

class EngineLoader;

/**
 * @brief Don't forget to define extern function:
 * extern "C"
 * {
 *     EXPORT Rendering * DLL_SINGLETON_LOADING_NAME(Rendering)();
 * }
 */
class COMMONFRAMEWORK_API Rendering : public DLL_Singleton<Rendering>
{
public:
    Rendering();

    static inline void SetDepthTest(bool enable) { _instance->_SetDepthTest(enable); }
    static inline void SetBlending(bool enable) { _instance->_SetBlending(enable); }
    enum class BlendingFunction
    {
        SRC_ALPHA,
        ONE_MINUS_SRC_ALPHA,
        ONE_MINUS_SRC_COLOR,
        ONE,
    };
    static inline void SetBlendingFunction(BlendingFunction src, BlendingFunction dst) { _instance->_SetBlendingFunction(src, dst); }
    enum class BlendingEquation
    {
        ADD,
        SUBTRACT,
    };
    static inline void SetBlendingEquation(BlendingEquation eq) { _instance->_SetBlendingEquation(eq); }
    static inline void SetDrawMode(const Drawable3D::DrawMode drawMode) { if (_lastDrawMode == drawMode) return; _instance->_SetDrawMode(drawMode); _lastDrawMode = drawMode; }
    static inline void DrawVertices(const VertexBuffer * vertices) { _instance->_DrawVertices(vertices); }

    static inline void SetGlobalDrawMode(const Drawable3D::DrawMode drawMode) { assert(drawMode != Drawable3D::DrawMode::GLOBAL); _globalDrawMode = drawMode; }
    static inline Drawable3D::DrawMode GetGlobalDrawMode() { return _globalDrawMode; }

    static inline void DrawModel(const Model* model) { _instance->_DrawModel(model); }
    static inline void ClearColorBuffer(const glm::vec4& color) { _instance->_ClearColorBuffer(color); }
    static inline void ClearDepthBuffer() { _instance->_ClearDepthBuffer(); }
    static inline void ClearStencilBuffer() { _instance->_ClearStencilBuffer(); }

protected:
    virtual void _SetDepthTest(bool enable) const = 0;
    virtual void _SetBlending(bool enable) const = 0;
    virtual void _SetBlendingFunction(BlendingFunction src, BlendingFunction dst) const = 0;
    virtual void _SetBlendingEquation(BlendingEquation eq) const = 0;
    virtual void _DrawVertices(const VertexBuffer * vertices) const = 0;
    virtual void _SetDrawMode(const Drawable3D::DrawMode drawMode) const = 0;
    virtual void _DrawModel(const Model* model) const = 0;
    virtual void _ClearColorBuffer(const glm::vec4& color) const = 0;
    virtual void _ClearDepthBuffer() const = 0;
    virtual void _ClearStencilBuffer() const = 0;
    static Drawable3D::DrawMode _globalDrawMode;
    static Drawable3D::DrawMode _lastDrawMode;
};