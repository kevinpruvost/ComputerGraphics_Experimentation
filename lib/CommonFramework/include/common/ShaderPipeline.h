#pragma once

#include <common/Shader.h>
#include <common/DLL.h>
#include <common/Drawable3D.h>
#include <common/Resources.h>

#include <memory>

class COMMONFRAMEWORK_API ShaderPipeline : public Resource
{
protected:
    ShaderPipeline();
public:
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

        static const char* GetTypeString(Type t)
        {
            switch (t)
            {
            case Type::MATRIX4:
                return "Matrix4";
            case Type::VEC3:
                return "Vec3";
            case Type::VEC4:
                return "Vec4";
            case Type::FLOAT:
                return "Float";
            case Type::INT:
                return "Int";
            }
            return "Unknown";
        }

        UniformVariable& operator= (const UniformVariable& other)
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
    struct UniformVariableSignature
    {
        std::string name;
        UniformVariable::Type type;
    };

    virtual ~ShaderPipeline() = default;
    virtual void SetPipeline(const std::vector<Shader *> & shaders) = 0;
    Venom::ErrorCode Use();
    virtual Venom::ErrorCode _Use() = 0;
    static ShaderPipeline* CreateShaderPipeline(const char * name, const std::vector<Shader*>& shaders);

#ifdef _DEBUG
    /**
     * @brief Gets Uniform variable signatures from the shader file
     */
    const std::vector<UniformVariableSignature> & GetUniformVariableSignatures();
#endif

    void SetUniformMatrix4(const std::string& name, const glm::mat4& matrix);
    void SetUniformVec3(const std::string& name, const glm::vec3& vec);
    void SetUniformVec4(const std::string& name, const glm::vec4& vec);
    void SetUniformFloat(const std::string& name, float value);
    void SetUniformInt(const std::string& name, int value);

    /**
     * @brief Gives Previously set uniform variables to other shaders
     * @param otherShader 
     */
    void GiveUniformVariablesToOtherShader(ShaderPipeline * otherShader);

    virtual void SetDrawMode(Drawable3D::DrawMode mode) = 0;

    static ShaderPipeline* GetCurrentlyUsedPipeline();

protected:
    virtual void _SetUniformMatrix4(const std::string& name, const glm::mat4& matrix) = 0;
    virtual void _SetUniformVec3(const std::string& name, const glm::vec3& vec) = 0;
    virtual void _SetUniformVec4(const std::string& name, const glm::vec4& vec) = 0;
    virtual void _SetUniformFloat(const std::string& name, float value) = 0;
    virtual void _SetUniformInt(const std::string& name, int value) = 0;

    virtual void _SetUniformVariableSignatures() = 0;

protected:
    std::unordered_map<std::string, UniformVariable> __uniformVariables;
#ifdef _DEBUG
    std::vector<UniformVariableSignature> __uniformVariableSignatures;
#endif
};