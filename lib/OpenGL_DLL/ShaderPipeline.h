#pragma once

#include <common/ShaderPipeline.h>
#include "Shader.h"

#include <glad/glad.h>

class ShaderPipeline_OGL : public ShaderPipeline
{
public:
    ShaderPipeline_OGL();

    void SetPipeline(const std::vector<Shader*> & shaders) override;
    ErrorCode Use() override;
private:
    GLuint m_program;

};

extern "C"
{
    EXPORT ShaderPipeline* createShaderPipeline();
}