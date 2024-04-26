#include <common/ShaderPipeline.h>
#include <common/Engine/EngineLoader.h>

typedef ShaderPipeline * (*CreateShaderPipelineFn)();

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
