#pragma once

#include <string>
#include <common/DLL.h>
#include <common/Exception.h>

class Shader {
public:
    enum class ShaderType {
        Vertex,
        Fragment,
        Geometry,
        TessControl,
        TessEvaluation,
        Compute
    };

    virtual ~Shader() = default;

    virtual void SetShaderSource(const std::string& shaderSource, const ShaderType type) = 0;
    static Shader * CreateShader(const std::string & shaderSource, const ShaderType type);
};