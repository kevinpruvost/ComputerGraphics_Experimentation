#include <common/ShaderPipeline.h>
#include <common/Engine/EngineLoader.h>

ShaderPipeline* currentlyUsedPipeline = nullptr;

ShaderPipeline::ShaderPipeline()
    : Resource(EngineResource::ResourceType::SHADER)
    , _hasTesselationStage{ false }
    , _hasGeometryStage{ false }
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

bool ShaderPipeline::HasTesselationStage() const
{
    return _hasTesselationStage;
}

bool ShaderPipeline::HasGeometryStage() const
{
    return _hasGeometryStage;
}

const std::vector<ShaderPipeline::UniformVariableSignature>& ShaderPipeline::GetUniformVariableSignatures()
{
    if (_uniformVariableSignatures.empty())
    {
        _SetUniformVariableSignatures();
#ifdef _DEBUG
        for (const auto& var : _uniformVariableSignatures)
            Logger::DebugPrint("Uniform variable:[%s]:\"%s\"", ShaderPipeline::UniformVariable::GetTypeString(var.type), var.name.c_str());
#endif
    }
    return _uniformVariableSignatures;
}

std::vector<ShaderPipeline::UniformVariableSignature>& ShaderPipeline::GetUniformVariableSignaturesRef()
{
    if (_uniformVariableSignatures.empty()) {
        _SetUniformVariableSignatures();
#ifdef _DEBUG
        for (const auto& var : _uniformVariableSignatures)
            Logger::DebugPrint("Uniform variable:[%s]:\"%s\"", ShaderPipeline::UniformVariable::GetTypeString(var.type), var.name.c_str());
#endif
    }
    return _uniformVariableSignatures;
}

void ShaderPipeline::SetUniformMatrix4(const char * name, const glm::mat4& matrix)
{
    Use();
    _SetUniformMatrix4(name, matrix);
    UniformVariable var = {
        UniformVariable::Type::MATRIX4
    };
    var.matrix = matrix;
    _uniformVariables[name] = var;
}

void ShaderPipeline::SetUniformVec3(const char * name, const glm::vec3& vec)
{
    Use();
    _SetUniformVec3(name, vec);
    UniformVariable var = {
        UniformVariable::Type::VEC3
    };
    var.vec3 = vec;
    _uniformVariables[name] = var;
}

void ShaderPipeline::SetUniformVec4(const char * name, const glm::vec4& vec)
{
    Use();
    _SetUniformVec4(name, vec);
    UniformVariable var = {
        UniformVariable::Type::VEC4
    };
    var.vec4 = vec;
    _uniformVariables[name] = var;
}

void ShaderPipeline::SetUniformFloat(const char * name, float value)
{
    Use();
    _SetUniformFloat(name, value);
    UniformVariable var = {
        UniformVariable::Type::FLOAT
    };
    var.f = value;
    _uniformVariables[name] = var;
}

void ShaderPipeline::SetUniformInt(const char * name, int value)
{
    Use();
    _SetUniformInt(name, value);
    UniformVariable var = {
        UniformVariable::Type::INT
    };
    var.i = value;
    _uniformVariables[name] = var;
}

void ShaderPipeline::GiveUniformVariablesToOtherShader(ShaderPipeline* otherShader)
{
    otherShader->Use();
    for (auto& [name, var] : _uniformVariables)
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

    const ConstYamlNode& path = data["path"];
    bool is_seq = path.is_seq();
    bool is_map = path.is_map();
    bool is_val = path.is_val();
    std::string fragPath, vertPath, tcsPath, tesPath, geomPath;
    if (is_seq) {
        for (int i = 0; i < path.num_children(); ++i) {
            String pathStr = YamlNodeToString(path[i]);
            if (pathStr.find(".frag") != std::string::npos) fragPath = pathStr;
            else if (pathStr.find(".vert") != std::string::npos) vertPath = pathStr;
            else if (pathStr.find(".tcs") != std::string::npos) tcsPath = pathStr;
            else if (pathStr.find(".tes") != std::string::npos) tesPath = pathStr;
            else if (pathStr.find(".geom") != std::string::npos) geomPath = pathStr;
        }
        if (fragPath.empty()) fragPath = fmt::format("Shaders/{}.frag", name);
        if (vertPath.empty()) vertPath = fmt::format("Shaders/{}.vert", name);
        if (tcsPath.empty()) tcsPath = fmt::format("Shaders/{}.tcs", name);
        if (tesPath.empty()) tesPath = fmt::format("Shaders/{}.tes", name);
        if (geomPath.empty()) geomPath = fmt::format("Shaders/{}.geom", name);
    }
    else
    {
        fragPath = fmt::format("Shaders/{}.frag", name);
        vertPath = fmt::format("Shaders/{}.vert", name);
        tcsPath = fmt::format("Shaders/{}.tcs", name);
        tesPath = fmt::format("Shaders/{}.tes", name);
        geomPath = fmt::format("Shaders/{}.geom", name);
    }

    auto frag = Shader::CreateShader(fragPath, Shader::ShaderType::Fragment);
    auto vert = Shader::CreateShader(vertPath, Shader::ShaderType::Vertex);
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

    return ShaderPipeline::CreateShaderPipeline(name, shaders);
}

std::unordered_map<const char *, ShaderPipeline::UniformVariable>& ShaderPipeline::GetUniformVariables()
{
    return _uniformVariables;
}

void ShaderPipeline::SetDefaultValuesForUniformVariables()
{
    if (_uniformVariableSignatures.size() != _uniformVariables.size())
    {
        if (_uniformVariableSignatures.empty())
            _SetUniformVariableSignatures();
        for (const auto& var : _uniformVariableSignatures)
        {
#ifdef _DEBUG
            Logger::DebugPrint("Uniform variable:[%s]:\"%s\"", ShaderPipeline::UniformVariable::GetTypeString(var.type), var.name.c_str());
#endif
            switch (var.type)
            {
            case UniformVariable::Type::FLOAT:
                SetUniformFloat(var.name.c_str(), 0.0f);
                break;
            case UniformVariable::Type::INT:
                SetUniformInt(var.name.c_str(), 0);
                break;
            case UniformVariable::Type::VEC3:
                SetUniformVec3(var.name.c_str(), glm::vec3(0.0f));
                break;
            case UniformVariable::Type::VEC4:
                SetUniformVec4(var.name.c_str(), glm::vec4(0.0f));
                break;
            case UniformVariable::Type::MATRIX4:
                SetUniformMatrix4(var.name.c_str(), glm::mat4(1.0f));
                break;
            }
        }
    }
}
