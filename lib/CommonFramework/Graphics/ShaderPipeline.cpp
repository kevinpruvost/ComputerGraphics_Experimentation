#include <common/ShaderPipeline.h>
#include <common/Engine/EngineLoader.h>

ShaderPipeline* currentlyUsedPipeline = nullptr;

Venom::ErrorCode ShaderPipeline::Use()
{
    Venom::ErrorCode err = _Use();
    if (err != Venom::ErrorCode::Success)
        return err;
    currentlyUsedPipeline = this;
}

typedef ShaderPipeline* (*CreateShaderPipelineFn)();
ShaderPipeline * ShaderPipeline::CreateShaderPipeline(const std::vector<Shader*>& shaders)
{
    CreateShaderPipelineFn createShaderFn = EngineLoader::GetEngineDll()->getFunction<CreateShaderPipelineFn>("createShaderPipeline");
    if (createShaderFn == nullptr)
        throw DLLException("Failed to load createShader function from engine dll");
    ShaderPipeline * shader = createShaderFn();
    if (shader == nullptr)
        throw DLLException("Failed to create shader");
    shader->SetPipeline(shaders);
    return shader;
}

ShaderPipeline* ShaderPipeline::GetCurrentlyUsedPipeline()
{
    return currentlyUsedPipeline;
}

void ShaderPipeline::SetUniformMatrix4(const std::string& name, const glm::mat4& matrix)
{
    _SetUniformMatrix4(name, matrix);
    UniformVariable var = {
        UniformVariable::Type::MATRIX4
    };
    var.matrix = matrix;
    m_uniformVariables[name] = var;
}

void ShaderPipeline::SetUniformVec3(const std::string& name, const glm::vec3& vec)
{
    _SetUniformVec3(name, vec);
    UniformVariable var = {
        UniformVariable::Type::VEC3
    };
    var.vec3 = vec;
    m_uniformVariables[name] = var;
}

void ShaderPipeline::SetUniformVec4(const std::string& name, const glm::vec4& vec)
{
    _SetUniformVec4(name, vec);
    UniformVariable var = {
        UniformVariable::Type::VEC4
    };
    var.vec4 = vec;
    m_uniformVariables[name] = var;
}

void ShaderPipeline::SetUniformFloat(const std::string& name, float value)
{
    _SetUniformFloat(name, value);
    UniformVariable var = {
        UniformVariable::Type::FLOAT
    };
    var.f = value;
    m_uniformVariables[name] = var;
}

void ShaderPipeline::SetUniformInt(const std::string& name, int value)
{
    _SetUniformInt(name, value);
    UniformVariable var = {
        UniformVariable::Type::INT
    };
    var.i = value;
    m_uniformVariables[name] = var;
}

void ShaderPipeline::GiveUniformVariablesToOtherShader(ShaderPipeline* otherShader)
{
    for (auto& [name, var] : m_uniformVariables)
    {
        switch (var.type)
        {
        case UniformVariable::Type::FLOAT:
            otherShader->SetUniformFloat(name, var.f);
            break;
        case UniformVariable::Type::INT:
            otherShader->SetUniformInt(name, var.i);
            break;
        case UniformVariable::Type::VEC3:
            otherShader->SetUniformVec3(name, var.vec3);
            break;
        case UniformVariable::Type::VEC4:
            otherShader->SetUniformVec4(name, var.vec4);
            break;
        case UniformVariable::Type::MATRIX4:
            otherShader->SetUniformMatrix4(name, var.matrix);
            break;
        }
    }
}

template<>
ShaderPipeline * Resources::_Load(const char const* name)
{
    auto frag = Shader::CreateShader(fmt::format("resources/Shaders/{}.frag", name), Shader::ShaderType::Fragment);
    auto vert = Shader::CreateShader(fmt::format("resources/Shaders/{}.vert", name), Shader::ShaderType::Vertex);
    return ShaderPipeline::CreateShaderPipeline({ frag, vert });
}