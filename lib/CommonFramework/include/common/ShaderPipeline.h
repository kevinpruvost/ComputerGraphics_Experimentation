#pragma once

#include <common/Shader.h>
#include <common/DLL.h>
#include <common/Drawable3D.h>
#include <common/Resources.h>

#include <memory>

class COMMONFRAMEWORK_API ShaderPipeline : public Resource
{
public:
    virtual ~ShaderPipeline() = default;
    virtual void SetPipeline(const std::vector<Shader *> & shaders) = 0;
    Venom::ErrorCode Use();
    virtual Venom::ErrorCode _Use() = 0;
    static ShaderPipeline* CreateShaderPipeline(const std::vector<Shader*>& shaders);

    void SetUniformMatrix4(const std::string& name, const glm::mat4& matrix);
    void SetUniformVec3(const std::string& name, const glm::vec3& vec);
    void SetUniformVec4(const std::string& name, const glm::vec4& vec);
    void SetUniformFloat(const std::string& name, float value);
    void SetUniformInt(const std::string& name, int value);

    void GiveUniformVariablesToOtherShader(ShaderPipeline * otherShader);

    virtual void SetDrawMode(Drawable3D::DrawMode mode) = 0;

    static ShaderPipeline* GetCurrentlyUsedPipeline();

protected:
    virtual void _SetUniformMatrix4(const std::string& name, const glm::mat4& matrix) = 0;
    virtual void _SetUniformVec3(const std::string& name, const glm::vec3& vec) = 0;
    virtual void _SetUniformVec4(const std::string& name, const glm::vec4& vec) = 0;
    virtual void _SetUniformFloat(const std::string& name, float value) = 0;
    virtual void _SetUniformInt(const std::string& name, int value) = 0;

private:
    struct UniformVariable
    {
        enum class Type
        {
            MATRIX4,
            VEC3,
            VEC4,
            FLOAT,
            INT
        };
        Type type;
        union
        {
            glm::mat4 matrix;
            glm::vec3 vec3;
            glm::vec4 vec4;
            float f;
            int i;
        };

        UniformVariable & operator= (const UniformVariable& other)
        {
            type = other.type;
            switch (other.type)
            {
            case Type::MATRIX4:
                matrix = other.matrix;
                break;
            case Type::VEC3:
                vec3 = other.vec3;
                break;
            case Type::VEC4:
                vec4 = other.vec4;
                break;
            case Type::FLOAT:
                f = other.f;
                break;
            case Type::INT:
                i = other.i;
                break;
            }
            return *this;
        }
    };
    std::unordered_map<std::string, UniformVariable> m_uniformVariables;
};