#pragma once

#include <string>
#include <common/DLL.h>
#include <common/Exception.h>
#include <glm/glm.hpp>

class Shader {
public:
    enum class ShaderType {
        Vertex = 0,
        Fragment,
        Geometry,
        TessControl,
        TessEvaluation,
        Compute,
        Count
    };

    virtual ~Shader() = default;

    virtual void SetShaderSource(const std::string& shaderSource, const ShaderType type) = 0;
    static Shader * CreateShader(const std::string & shaderSource, const ShaderType type);

    virtual void SetUniformMatrix4(const std::string & name, const glm::mat4 & matrix) = 0;
    virtual void SetUniformVec3(const std::string & name, const glm::vec3 & vec) = 0;
    virtual void SetUniformVec4(const std::string& name, const glm::vec4& vec) = 0;

};