#pragma once

#include <common/Shader.h>

class ShaderPipeline
{
public:
    virtual ~ShaderPipeline() = default;

    virtual void SetPipeline(const std::vector<Shader *> & shaders) = 0;
    virtual ErrorCode Use() = 0;
    static ShaderPipeline* CreateShaderPipeline(const std::vector<Shader*>& shaders);
};