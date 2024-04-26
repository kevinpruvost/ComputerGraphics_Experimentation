#include <common/Shader.h>
#include <common/Engine/EngineLoader.h>

typedef Shader* (*CreateShaderFn)();

Shader* Shader::CreateShader(const std::string & shaderSource, const ShaderType type)
{
    CreateShaderFn createShaderFn = EngineLoader::GetEngineDll()->getFunction<CreateShaderFn>("createShader");
    if (createShaderFn == nullptr)
        throw DLLException("Failed to load createShader function from engine dll");
    Shader * shader = createShaderFn();
    if (shader == nullptr)
        throw DLLException("Failed to create shader");
    shader->SetShaderSource(shaderSource, type);
    return shader;
}