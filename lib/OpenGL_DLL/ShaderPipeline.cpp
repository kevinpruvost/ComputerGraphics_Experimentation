#include "ShaderPipeline.h"

#include <common/Common.h>
#include <common/Math_Base.h>

ShaderPipeline_OGL::ShaderPipeline_OGL()
    : m_program{ 0 }
{
}

void ShaderPipeline_OGL::SetPipeline(const std::vector<Shader*>& shaders)
{
    // Create the program
    m_program = glCreateProgram();

    // Attach the shaders
    for (auto shader : shaders)
    {
        glAttachShader(m_program, reinterpret_cast<Shader_OGL *>(shader)->GetShaderId());
    }

    // Link the program
    glLinkProgram(m_program);

    // Check for errors
    GLint success = GL_TRUE;
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLchar infoLog[512];
        glGetProgramInfoLog(m_program, 512, NULL, infoLog);
        Logger::Print("ERROR::SHADER::PROGRAM::LINKING_FAILED%s", infoLog);
    }

    // Detach the shaders
    for (auto shader : shaders)
    {
        glDetachShader(m_program, reinterpret_cast<Shader_OGL *>(shader)->GetShaderId());
    }
}

Venom::ErrorCode ShaderPipeline_OGL::_Use()
{
    glUseProgram(m_program);
    return Venom::ErrorCode::Success;
}

void ShaderPipeline_OGL::_SetUniformMatrix4(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(GetUniformLocation(name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderPipeline_OGL::_SetUniformVec3(const std::string& name, const glm::vec3& vec)
{
    glUniform3fv(GetUniformLocation(name.c_str()), 1, glm::value_ptr(vec));
}

void ShaderPipeline_OGL::_SetUniformVec4(const std::string& name, const glm::vec4& vec)
{
    glUniform4fv(GetUniformLocation(name.c_str()), 1, glm::value_ptr(vec));
}

void ShaderPipeline_OGL::_SetUniformFloat(const std::string& name, float value)
{
    glUniform1f(GetUniformLocation(name.c_str()), value);
}

void ShaderPipeline_OGL::_SetUniformInt(const std::string& name, int value)
{
    glUniform1i(GetUniformLocation(name.c_str()), value);
}

void ShaderPipeline_OGL::SetDrawMode(Drawable3D::DrawMode mode)
{
    switch (mode)
    {
    case Drawable3D::DrawMode::POINTS:
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        SetUniformInt("useVertColor", 1);
        break;
    case Drawable3D::DrawMode::WIREFRAME:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        SetUniformInt("useVertColor", 1);
        break;
    case Drawable3D::DrawMode::SOLID:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        SetUniformInt("useVertColor", 0);
        break;
    }
}

// Export the factory function to create an instance of the class
EXPORT ShaderPipeline* createShaderPipeline() {
    return new ShaderPipeline_OGL();
}