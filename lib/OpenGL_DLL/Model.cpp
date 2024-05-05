#include "Model.h"
#include "Mesh.h"
#include "BufferManager.h"

#include <common/ShaderPipeline.h>
#include <common/Common.h>

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
	_vertexBuffer->Bind();
	ShaderPipeline * shader = ShaderPipeline::GetCurrentlyUsedPipeline(), * wireframeShader = nullptr;
	assert(shader);
	if (_drawMode & DrawMode::SOLID)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, _vertexBuffer->GetVertexCount());
	}
	if (_drawMode & DrawMode::WIREFRAME || _drawMode & DrawMode::POINTS)
	{
		wireframeShader = Resources::Load<ShaderPipeline>("Wireframe");
		wireframeShader->Use();
        shader->GiveUniformVariablesToOtherShader(wireframeShader);
	}
	if (_drawMode & DrawMode::WIREFRAME)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, _vertexBuffer->GetVertexCount());
	}
	if (_drawMode & DrawMode::POINTS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glDrawArrays(GL_TRIANGLES, 0, _vertexBuffer->GetVertexCount());
    }
	shader->Use();
	//for (auto & m : _meshes)
	//{
	//	Mesh_OGL * mesh = dynamic_cast<Mesh_OGL *>(m.get());
	//	const TriangleArray & triangles = mesh->GetTriangles();
	//	glDrawElements(GL_TRIANGLES,       // mode
	//		mesh->GetTriangles().size(),   // number of indices
	//		GL_UNSIGNED_INT,               // type of the indices
	//	    triangles.data()               // pointer to the indices
	//	);
	//	mesh->Draw();
	//}
	_vertexBuffer->Unbind();
}

Venom::ErrorCode Model_OGL::ReloadObjectFromEngine()
{
	return Venom::ErrorCode::Success;
}
