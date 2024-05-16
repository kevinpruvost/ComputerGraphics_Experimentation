#pragma once

#include <common/Mesh.h>
#include <common/Texture.h>
#include <common/Model.h>

class Text2D
{
public:
    ~Text2D() = default;

    static Text2D* CreateText2D(const char * fontPath, int fontSize);

    virtual void RenderText(const char* text, float x, float y, float scale, const glm::vec3& color) = 0;
    virtual void InitText2D(const char* fontPath, int fontSize) = 0;

protected:
    Text2D() = default;
private:
};

