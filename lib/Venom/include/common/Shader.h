#pragma once

#include <string>
#include <common/DLL.h>
#include <common/Exception.h>
#include <common/Math_Base.h>

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
    VENOM_API static Shader * CreateShader(const std::string & shaderSource, const ShaderType type);
};