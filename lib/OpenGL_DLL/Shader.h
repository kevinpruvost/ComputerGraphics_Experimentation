#pragma once

#include <common/Shader.h>
#include <glad/glad.h>

class Shader_OGL : public Shader
{
public:
    Shader_OGL() = default;

    void SetShaderSource(const std::string& shaderSource, const ShaderType type) override;

    GLuint GetShaderId() const;

private:
    std::string m_shaderSource;
    GLuint m_shaderId;
};

extern "C"
{
    EXPORT Shader * createShader();
}