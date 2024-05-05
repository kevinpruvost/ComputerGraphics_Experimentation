#pragma once

#include <common/DLL.h>
#include <common/Memory.h>
#include <common/Vertex.h>
#include <common/Drawable3D.h>

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
        Add,
        Subtract,
    };
    static inline void SetBlendingEquation(BlendingEquation eq) { _instance->_SetBlendingEquation(eq); }
    static inline void SetDrawMode(const Drawable3D::DrawMode drawMode) { _instance->_SetDrawMode(drawMode); }
    static inline void DrawVertices(const VertexBuffer * vertices) { _instance->_DrawVertices(vertices); }

protected:
    virtual void _SetDepthTest(bool enable) const = 0;
    virtual void _SetBlendingFunction(BlendingFunction src, BlendingFunction dst) const = 0;
    virtual void _SetBlendingEquation(BlendingEquation eq) const = 0;
    virtual void _DrawVertices(const VertexBuffer * vertices) const = 0;
    virtual void _SetDrawMode(const Drawable3D::DrawMode drawMode) const = 0;
};