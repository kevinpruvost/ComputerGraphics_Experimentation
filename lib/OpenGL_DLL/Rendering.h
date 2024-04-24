#pragma once

#include <common/Rendering.h>
#include <glad/glad.h>


class Rendering_OGL : public Rendering
{
public:
    virtual void _SetDepthTest(bool enable) const
    {
        if (enable)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
    }
};

extern "C"
{
    EXPORT Rendering* DLL_SINGLETON_LOADING_NAME(Rendering)();
}