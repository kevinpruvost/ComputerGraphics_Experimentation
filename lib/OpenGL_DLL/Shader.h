#pragma once

#include <common/Shader.h>
#include <glad/glad.h>
#include <unordered_map>

class Shader_OGL : public Shader
{
public:
    Shader_OGL();
    ~Shader_OGL();

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