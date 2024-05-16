#include "Shader.h"

#include <common/Math_Base.h>

Shader_OGL::Shader_OGL()
    : m_shaderId(0)
    , m_shaderSource()
{
}

Shader_OGL::~Shader_OGL()
{
    if (m_shaderId != 0)
    {
        glDeleteShader(m_shaderId);
    }
}

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
        case ShaderType::TessControl:
            m_shaderId = glCreateShader(GL_TESS_CONTROL_SHADER);
            break;
        case ShaderType::TessEvaluation:
            m_shaderId = glCreateShader(GL_TESS_EVALUATION_SHADER);
            break;
        case ShaderType::Geometry:
            m_shaderId = glCreateShader(GL_GEOMETRY_SHADER);
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
    GLint success = GL_TRUE;
    GLchar infoLog[512];
    glGetShaderiv(m_shaderId, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
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