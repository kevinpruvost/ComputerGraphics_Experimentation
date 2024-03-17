#include "ShaderPipeline.h"

#include <common/Common.h>

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
    GLint success;
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[512];
        glGetProgramInfoLog(m_program, 512, NULL, infoLog);
        Logger::Print("ERROR::SHADER::PROGRAM::LINKING_FAILED{0}", infoLog);
    }

    // Detach the shaders
    //for (auto shader : shaders)
    //{
    //    glDetachShader(m_program, reinterpret_cast<Shader_OGL *>(shader)->GetShaderId());
    //}
}

ErrorCode ShaderPipeline_OGL::Use()
{
    glUseProgram(m_program);
    return ErrorCode::Success;
}

// Export the factory function to create an instance of the class
EXPORT ShaderPipeline* createShaderPipeline() {
    return new ShaderPipeline_OGL();
}