#include "Model.h"
#include "Mesh.h"
#include "BufferManager.h"

#include <common/ShaderPipeline.h>
#include <common/Common.h>
#include <common/Rendering.h>

// Export the factory function to create an instance of the class
EXPORT Model * createModel() {
    return new Model_OGL();
}

Model_OGL::Model_OGL()
{
}

void Model_OGL::SetIndices(const TriangleArray& indices)
{
}

void Model_OGL::Draw()
{
	ShaderPipeline* shader = ShaderPipeline::GetCurrentlyUsedPipeline(), * wireframeShader = nullptr;
	assert(shader);
	DrawMode drawMode = (_drawMode == DrawMode::GLOBAL) ? Rendering::GetGlobalDrawMode() : _drawMode;
	if (drawMode & DrawMode::WIREFRAME || drawMode & DrawMode::POINTS)
	{
		wireframeShader = Resources::Load<ShaderPipeline>("Wireframe");
		wireframeShader->Use();
		shader->GiveUniformVariablesToOtherShader(wireframeShader);
	}
	for (const auto & m : _meshes)
	{
		Mesh_OGL* mesh = dynamic_cast<Mesh_OGL*>(m.get());
		shader->Use();
		const VertexBuffer* vertexBuffer = mesh->GetVertexBuffer();
		vertexBuffer->Bind();
		if (mesh->GetMaterialId() <= _materials.size())
		{
			const Material* material = _materials[mesh->GetMaterialId()];
			const std::vector<Ptr<Texture>>& textures = material->GetTextures();
			for (int i = 0; i < textures.size(); i++)
			{
				textures[i]->BindTexture(Texture::TextureType::Texture2D, i);
				shader->SetUniformInt("textureSampler" + std::to_string(i), i);
			}
		}

		if (drawMode & DrawMode::SOLID)
		{
			Rendering::SetDrawMode(DrawMode::SOLID);
			vertexBuffer->Draw();
		}

		if (drawMode & DrawMode::WIREFRAME || drawMode & DrawMode::POINTS)
		{
			wireframeShader->Use();
			shader->GiveUniformVariablesToOtherShader(wireframeShader);
		}
		if (drawMode & DrawMode::WIREFRAME)
		{
			Rendering::SetDrawMode(DrawMode::WIREFRAME);
			vertexBuffer->Draw();
		}
		if (drawMode & DrawMode::POINTS)
		{
			Rendering::SetDrawMode(DrawMode::POINTS);
			vertexBuffer->Draw();
		}
	}
}

Venom::ErrorCode Model_OGL::ReloadObjectFromEngine()
{
	return Venom::ErrorCode::Success;
}
