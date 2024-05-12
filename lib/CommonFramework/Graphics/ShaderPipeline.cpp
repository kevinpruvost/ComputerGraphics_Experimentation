#include <common/ShaderPipeline.h>
#include <common/Engine/EngineLoader.h>

ShaderPipeline* currentlyUsedPipeline = nullptr;

ShaderPipeline::ShaderPipeline()
    : Resource(EngineResource::ResourceType::SHADER)
{
}

Venom::ErrorCode ShaderPipeline::Use()
{
    if (currentlyUsedPipeline == this)
        return Venom::ErrorCode::Success;
    Venom::ErrorCode err = _Use();
    if (err != Venom::ErrorCode::Success)
        return err;
    currentlyUsedPipeline = this;
    return Venom::ErrorCode::Success;
}

typedef ShaderPipeline* (*CreateShaderPipelineFn)();
ShaderPipeline * ShaderPipeline::CreateShaderPipeline(const char * name, const std::vector<Shader*>& shaders)
{
    CreateShaderPipelineFn createShaderFn = EngineLoader::GetEngineDll()->getFunction<CreateShaderPipelineFn>("createShaderPipeline");
    assert(createShaderFn != nullptr);
    ShaderPipeline * shader = createShaderFn();
    assert(shader != nullptr);
    shader->SetPipeline(shaders);
    shader->SetResourceName(name);
    return shader;
}

ShaderPipeline* ShaderPipeline::GetCurrentlyUsedPipeline()
{
    return currentlyUsedPipeline;
}

const std::vector<ShaderPipeline::UniformVariableSignature>& ShaderPipeline::GetUniformVariableSignatures()
{
    if (__uniformVariableSignatures.empty())
    {
        _SetUniformVariableSignatures();
#ifdef _DEBUG
        for (const auto& var : __uniformVariableSignatures)
            Logger::DebugPrint("Uniform variable:[%s]:\"%s\"", ShaderPipeline::UniformVariable::GetTypeString(var.type), var.name.c_str());
#endif
    }
    return __uniformVariableSignatures;
}

std::vector<ShaderPipeline::UniformVariableSignature>& ShaderPipeline::GetUniformVariableSignaturesRef()
{
    if (__uniformVariableSignatures.empty()) {
        _SetUniformVariableSignatures();
#ifdef _DEBUG
        for (const auto& var : __uniformVariableSignatures)
            Logger::DebugPrint("Uniform variable:[%s]:\"%s\"", ShaderPipeline::UniformVariable::GetTypeString(var.type), var.name.c_str());
#endif
    }
    return __uniformVariableSignatures;
}

void ShaderPipeline::SetUniformMatrix4(const std::string& name, const glm::mat4& matrix)
{
    _SetUniformMatrix4(name, matrix);
    UniformVariable var = {
        UniformVariable::Type::MATRIX4
    };
    var.matrix = matrix;
    __uniformVariables[name] = var;
}

void ShaderPipeline::SetUniformVec3(const std::string& name, const glm::vec3& vec)
{
    _SetUniformVec3(name, vec);
    UniformVariable var = {
        UniformVariable::Type::VEC3
    };
    var.vec3 = vec;
    __uniformVariables[name] = var;
}

void ShaderPipeline::SetUniformVec4(const std::string& name, const glm::vec4& vec)
{
    _SetUniformVec4(name, vec);
    UniformVariable var = {
        UniformVariable::Type::VEC4
    };
    var.vec4 = vec;
    __uniformVariables[name] = var;
}

void ShaderPipeline::SetUniformFloat(const std::string& name, float value)
{
    _SetUniformFloat(name, value);
    UniformVariable var = {
        UniformVariable::Type::FLOAT
    };
    var.f = value;
    __uniformVariables[name] = var;
}

void ShaderPipeline::SetUniformInt(const std::string& name, int value)
{
    _SetUniformInt(name, value);
    UniformVariable var = {
        UniformVariable::Type::INT
    };
    var.i = value;
    __uniformVariables[name] = var;
}

void ShaderPipeline::GiveUniformVariablesToOtherShader(ShaderPipeline* otherShader)
{
    for (auto& [name, var] : __uniformVariables)
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
ShaderPipeline * Resources::_Load(const char const* name, const YamlNode & data)
{
    Logger::DebugPrint("Loading shader pipeline: %s", name);

    std::string fragPath = fmt::format("Shaders/{}.frag", name);
    std::string vertPath = fmt::format("Shaders/{}.vert", name);
    std::string tcsPath = fmt::format("Shaders/{}.tcs", name);
    std::string tesPath = fmt::format("Shaders/{}.tes", name);
    std::string geomPath = fmt::format("Shaders/{}.geom", name);

    auto frag = Shader::CreateShader(fmt::format("Shaders/{}.frag", name), Shader::ShaderType::Fragment);
    auto vert = Shader::CreateShader(fmt::format("Shaders/{}.vert", name), Shader::ShaderType::Vertex);
    std::vector<Shader*> shaders{ frag, vert };

    // Check if Tessellation Control Shader (TCS) exists
    if (std::filesystem::exists(tcsPath)) {
        auto tcs = Shader::CreateShader(tcsPath.c_str(), Shader::ShaderType::TessControl);
        shaders.push_back(tcs);
    }

    // Check if Tessellation Evaluation Shader (TES) exists
    if (std::filesystem::exists(tesPath)) {
        auto tes = Shader::CreateShader(tesPath.c_str(), Shader::ShaderType::TessEvaluation);
        shaders.push_back(tes);
    }

    // Check if Geometry Shader (GS) exists
    if (std::filesystem::exists(geomPath)) {
        auto geom = Shader::CreateShader(geomPath.c_str(), Shader::ShaderType::Geometry);
        shaders.push_back(geom);
    }

    return ShaderPipeline::CreateShaderPipeline(name, { frag, vert });
}

std::unordered_map<std::string, ShaderPipeline::UniformVariable>& ShaderPipeline::GetUniformVariables()
{
    return __uniformVariables;
}

void ShaderPipeline::SetDefaultValuesForUniformVariables()
{
    if (__uniformVariableSignatures.size() != __uniformVariables.size())
    {
        if (__uniformVariableSignatures.empty())
            _SetUniformVariableSignatures();
        for (const auto& var : __uniformVariableSignatures)
        {
#ifdef _DEBUG
            Logger::DebugPrint("Uniform variable:[%s]:\"%s\"", ShaderPipeline::UniformVariable::GetTypeString(var.type), var.name.c_str());
#endif
            switch (var.type)
            {
            case UniformVariable::Type::FLOAT:
                SetUniformFloat(var.name, 0.0f);
                break;
            case UniformVariable::Type::INT:
                SetUniformInt(var.name, 0);
                break;
            case UniformVariable::Type::VEC3:
                SetUniformVec3(var.name, glm::vec3(0.0f));
                break;
            case UniformVariable::Type::VEC4:
                SetUniformVec4(var.name, glm::vec4(0.0f));
                break;
            case UniformVariable::Type::MATRIX4:
                SetUniformMatrix4(var.name, glm::mat4(1.0f));
                break;
            }
        }
    }
}
