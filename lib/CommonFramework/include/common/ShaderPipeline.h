#pragma once

#include <common/Shader.h>
#include <common/DLL.h>

#include <memory>

class COMMONFRAMEWORK_API ShaderPipeline
{
public:
    virtual ~ShaderPipeline() = default;
    virtual void SetPipeline(const std::vector<Shader *> & shaders) = 0;
    virtual ErrorCode Use() = 0;
    static ShaderPipeline* CreateShaderPipeline(const std::vector<Shader*>& shaders);

    virtual void SetUniformMatrix4(const std::string& name, const glm::mat4& matrix) = 0;
    virtual void SetUniformVec3(const std::string& name, const glm::vec3& vec) = 0;
    virtual void SetUniformVec4(const std::string& name, const glm::vec4& vec) = 0;
    virtual void SetUniformFloat(const std::string& name, float value) = 0;
    virtual void SetUniformInt(const std::string& name, int value) = 0;

    enum class DrawMode
    {
        POINTS,
        LINES,
        TRIANGLES,
        QUADS,
        POLYGON
    };
    virtual void SetDrawMode(DrawMode mode) = 0;
};