#pragma once

#include <common/Rendering.h>
#include <glad/glad.h>


class Rendering_OGL : public Rendering
{
public:
    void _SetDepthTest(bool enable) const override;
    void _SetBlendingFunction(BlendingFunction src, BlendingFunction dst) const override;
    void _SetBlendingEquation(BlendingEquation eq) const override;
};

extern "C"
{
    EXPORT Rendering* DLL_SINGLETON_LOADING_NAME(Rendering)();
}