#pragma once

#include <common/Shader.h>
#include <glad/glad.h>
#include <unordered_map>

class Shader_OGL : public Shader
{
public:
    Shader_OGL() = default;

    void SetShaderSource(const std::string& shaderSource, const ShaderType type) override;

    void SetUniformMatrix4(const std::string& name, const glm::mat4& matrix) override;
    void SetUniformVec3(const std::string& name, const glm::vec3& vec) override;
    void SetUniformVec4(const std::string& name, const glm::vec4& vec) override;

    GLuint GetShaderId() const;

    inline GLint & GetUniformLocation(const char * name) {
        auto ptr = m_uniformLocations.find(name);
        if (ptr == m_uniformLocations.end()) {
            m_uniformLocations[name] = glGetUniformLocation(m_shaderId, name);
            return m_uniformLocations.at(name);
        }
        return ptr->second;
    }

private:
    std::string m_shaderSource;
    GLuint m_shaderId;
    std::unordered_map<const char *, GLint> m_uniformLocations;
};

extern "C"
{
    EXPORT Shader * createShader();
}