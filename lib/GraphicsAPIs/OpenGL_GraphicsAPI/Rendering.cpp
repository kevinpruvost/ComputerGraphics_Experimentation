#include "Rendering.h"

#include "Mesh.h"
#include "ShaderPipeline.h"

EXPORT Rendering* DLL_SINGLETON_LOADING_NAME(Rendering)()
{
    return new Rendering_OGL();
}

void Rendering_OGL::_SetDepthTest(bool enable) const
{
    if (enable)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

void Rendering_OGL::_SetBlending(bool enable) const
{
	if (enable)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);
}

static GLenum GetGLBlendingFunction(Rendering::BlendingFunction func)
{
    switch (func)
    {
    case Rendering::BlendingFunction::ONE:
        return GL_ONE;
    case Rendering::BlendingFunction::ONE_MINUS_SRC_ALPHA:
        return GL_ONE_MINUS_SRC_ALPHA;
    case Rendering::BlendingFunction::SRC_ALPHA:
        return GL_SRC_ALPHA;
    case Rendering::BlendingFunction::ONE_MINUS_SRC_COLOR:
        return GL_ONE_MINUS_SRC_COLOR;
    }
}

void Rendering_OGL::_SetBlendingFunction(BlendingFunction src, BlendingFunction dst) const
{
    glBlendFunc(GetGLBlendingFunction(src), GetGLBlendingFunction(dst));
}

static GLenum GetGLBlendingEquation(Rendering::BlendingEquation eq)
{
    switch (eq)
    {
    case Rendering::BlendingEquation::ADD:
        return GL_FUNC_ADD;
    case Rendering::BlendingEquation::SUBTRACT:
        return GL_FUNC_SUBTRACT;
    }
}

void Rendering_OGL::_SetBlendingEquation(BlendingEquation eq) const
{
    glBlendEquation(GetGLBlendingEquation(eq));
}

void Rendering_OGL::_DrawTriangles(const VertexBuffer * vertices) const
{
    vertices->Bind();
    glDrawArrays(GL_TRIANGLES, 0, vertices->GetVertexCount());
    vertices->Unbind();
}

void Rendering_OGL::_DrawLines(const VertexBuffer* vertices) const
{
	vertices->Bind();
	glLineWidth(3.0f);
    glDrawArrays(GL_LINES, 0, vertices->GetVertexCount());
    vertices->Unbind();
}

void Rendering_OGL::_SetDrawMode(const Drawable3D::DrawMode drawMode) const
{
    switch (drawMode)
    {
    case Drawable3D::DrawMode::SOLID:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    case Drawable3D::DrawMode::WIREFRAME:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case Drawable3D::DrawMode::POINTS:
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        glPointSize(5.0f);
        break;
    default:
        assert(false && "Can only do SOLID, WIREFRAME or POINTS");
        break;
    }
}

void Rendering_OGL::_DrawModel(const Model* model) const
{
	ShaderPipeline * shader = model->GetShader();
	if (!shader) shader = ShaderPipeline::GetCurrentlyUsedPipeline();
	
	ShaderPipeline * wireframeShader = model->GetWireframeShader();

	assert(shader);
	Drawable3D::DrawMode drawMode = model->GetDrawMode();
	drawMode = (drawMode == Drawable3D::DrawMode::GLOBAL) ? Rendering::GetGlobalDrawMode() : drawMode;
	if (drawMode & Drawable3D::DrawMode::WIREFRAME || drawMode & Drawable3D::DrawMode::POINTS)
	{
		if (!wireframeShader) wireframeShader = Resources::Load<ShaderPipeline>("Wireframe");
	}
	const auto & materials = model->GetMaterials();
	for (const auto& m : model->GetMeshes())
	{
		Mesh_OGL* mesh = dynamic_cast<Mesh_OGL*>(m.get());
		shader->Use();
		const VertexBuffer* vertexBuffer = mesh->GetVertexBuffer();
		vertexBuffer->Bind();
		if (shader->HasTesselationStage() && vertexBuffer->GetVertexCount() < GL_MAX_PATCH_VERTICES) {
			glPatchParameteri(GL_PATCH_VERTICES, vertexBuffer->GetVertexCount());
		}
		if (mesh->GetMaterialId() <= materials.size())
		{
			const Material* material = materials[mesh->GetMaterialId()];
			const std::vector<Ptr<Texture>>& textures = material->GetTextures();
			if (model->IsUsingPBR())
			{
				// Set ambient properties
				if (material->IsUsingAmbientTexture()) {
					shader->SetUniformInt("material.useAmbientMap", true);
					material->GetAmbientTexture()->BindTexture(Texture::TextureType::Texture2D, 0);
					shader->SetUniformInt("material.ambientMap", 0);
				}
				else {
					shader->SetUniformInt("material.useAmbientMap", false);
					shader->SetUniformVec4("material.ambientColor", material->GetAmbient());
				}

				// Set diffuse properties
				if (material->IsUsingDiffuseTexture()) {
					shader->SetUniformInt("material.useDiffuseMap", true);
					material->GetDiffuseTexture()->BindTexture(Texture::TextureType::Texture2D, 1);
					shader->SetUniformInt("material.diffuseMap", 1);
				}
				else {
					shader->SetUniformInt("material.useDiffuseMap", false);
					shader->SetUniformVec4("material.diffuseColor", material->GetDiffuse());
				}

				// Set specular properties
				if (material->IsUsingSpecularTexture()) {
					shader->SetUniformInt("material.useSpecularMap", true);
					material->GetSpecularTexture()->BindTexture(Texture::TextureType::Texture2D, 2);
					shader->SetUniformInt("material.specularMap", 2);
				}
				else {
					shader->SetUniformInt("material.useSpecularMap", false);
					shader->SetUniformVec4("material.specularColor", material->GetSpecular());
				}

				// Set normal map properties
				if (material->IsUsingNormalTexture()) {
					shader->SetUniformInt("material.useNormalMap", true);
					material->GetNormalTexture()->BindTexture(Texture::TextureType::Texture2D, 3);
					shader->SetUniformInt("material.normalMap", 3);
				}
				else {
					shader->SetUniformInt("material.useNormalMap", false);
				}

				// Set Base Color properties
				if (material->IsUsingBaseColorTexture()) {
					shader->SetUniformInt("material.useBaseColorMap", true);
					material->GetBaseColorTexture()->BindTexture(Texture::TextureType::Texture2D, 4);
					shader->SetUniformInt("material.baseColorMap", 4);
				}
				else {
					shader->SetUniformInt("material.useBaseColorMap", false);
					shader->SetUniformVec4("material.baseColor", material->GetBaseColor());
				}

				// Set shininess
				shader->SetUniformFloat("material.shininess", material->GetShininess());
			}
			else
			{
				char samplerName[32] = "textureSampler[0]\0"; // [0] is replaced with [i]
				for (int i = 0; i < textures.size(); i++)
				{
					textures[i]->BindTexture(Texture::TextureType::Texture2D, i);
					samplerName[13] = i + '0';
					shader->SetUniformInt(samplerName, i);
				}
			}
		}

		if (drawMode & Drawable3D::DrawMode::SOLID)
		{
			Rendering::SetDrawMode(Drawable3D::DrawMode::SOLID);
			vertexBuffer->Draw();
		}

		if (drawMode & Drawable3D::DrawMode::WIREFRAME || drawMode & Drawable3D::DrawMode::POINTS)
		{
			shader->GiveUniformVariablesToOtherShader(wireframeShader);
			wireframeShader->Use();
		}
		if (drawMode & Drawable3D::DrawMode::WIREFRAME)
		{
			Rendering::SetDrawMode(Drawable3D::DrawMode::WIREFRAME);
			vertexBuffer->Draw();
		}
		if (drawMode & Drawable3D::DrawMode::POINTS)
		{
			Rendering::SetDrawMode(Drawable3D::DrawMode::POINTS);
			vertexBuffer->Draw();
		}
	}
}

void Rendering_OGL::_ClearDepthBuffer() const
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Rendering_OGL::_ClearStencilBuffer() const
{
	glClear(GL_STENCIL_BUFFER_BIT);
}

void Rendering_OGL::_ClearColorBuffer(const glm::vec4& color) const
{
	glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Rendering_OGL::_RenderLights(const std::vector<Light*>* lights) const
{
	ShaderPipeline* currentlyUsed = ShaderPipeline::GetCurrentlyUsedPipeline();
	currentlyUsed->SetUniformInt("lightCount", lights->size());

	static char uniformName1[32] = "light[0].position\0";
	static char uniformName2[32] = "light[0].color\0";
	static char uniformName3[32] = "light[0].intensity\0";
	static char uniformName4[32] = "light[0].attenuation\0";
	for (int index = 0; index < lights->size(); ++index)
	{
		{
			uniformName1[6] = index + '0';
			currentlyUsed->SetUniformVec3(uniformName1, lights->at(0)->GetPosition());
		}
		{
			uniformName2[6] = index + '0';
			currentlyUsed->SetUniformVec3(uniformName2, lights->at(0)->GetColor());
		}
		{
			uniformName3[6] = index + '0';
			currentlyUsed->SetUniformFloat(uniformName3, lights->at(0)->GetIntensity());
		}
		{
			uniformName4[6] = index + '0';
			currentlyUsed->SetUniformFloat(uniformName4, lights->at(0)->GetAttenuation());
		}
	}
}
