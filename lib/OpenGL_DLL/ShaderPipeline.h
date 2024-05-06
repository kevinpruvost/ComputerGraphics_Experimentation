#pragma once

#include <common/ShaderPipeline.h>
#include "Shader.h"

#include <glad/glad.h>

class ShaderPipeline_OGL : public ShaderPipeline
{
public:
    ShaderPipeline_OGL();

    void SetPipeline(const std::vector<Shader*> & shaders) override;
    Venom::ErrorCode _Use() override;

    void _SetUniformMatrix4(const std::string& name, const glm::mat4& matrix) override;
    void _SetUniformVec3(const std::string& name, const glm::vec3& vec) override;
    void _SetUniformVec4(const std::string& name, const glm::vec4& vec) override;
    void _SetUniformFloat(const std::string& name, float value) override;
    void _SetUniformInt(const std::string& name, int value) override;

    void SetDrawMode(Drawable3D::DrawMode mode) override;

    Venom::ErrorCode ReloadObjectFromEngine() override;

private:
#ifdef _DEBUG
    void _SetUniformVariableSignatures() override;
#endif


    GLuint m_program;

    std::unordered_map<const char*, GLint> m_uniformLocations;
    GLint& GetUniformLocation(const char* name) {
        auto ptr = m_uniformLocations.find(name);
        if (ptr == m_uniformLocations.end()) {
            m_uniformLocations[name] = glGetUniformLocation(m_program, name);
            return m_uniformLocations.at(name);
        }
        return ptr->second;
    }
};

extern "C"
{
    EXPORT ShaderPipeline* createShaderPipeline();
}