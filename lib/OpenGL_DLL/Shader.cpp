#include "Shader.h"

void Shader_OGL::SetShaderSource(const std::string& shaderSource, const ShaderType type)
{
    m_shaderSource = shaderSource;

    switch (type)
    {
        case ShaderType::Vertex:
            m_shaderId = glCreateShader(GL_VERTEX_SHADER);
            break;
        case ShaderType::Fragment:
            m_shaderId = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        default:
            throw NotImplementedException("Shader type not implemented");
            break;
    }

    // Read shader file
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        shaderFile.open(m_shaderSource);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        m_shaderSource = shaderStream.str();
    }
    catch (const std::ifstream::failure & e)
    {
        throw RuntimeException("Shader file not succesfully read");
    }
    const GLchar * shaderContent = m_shaderSource.c_str();

    glShaderSource(m_shaderId, 1, &shaderContent, NULL);
    glCompileShader(m_shaderId);
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(m_shaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(m_shaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

GLuint Shader_OGL::GetShaderId() const
{
    return m_shaderId;
}

// Export the factory function to create an instance of the class
EXPORT Shader* createShader() {
    return new Shader_OGL();
}