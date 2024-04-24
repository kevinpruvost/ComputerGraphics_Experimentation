#include "Model.h"
#include "Mesh.h"
#include "BufferManager.h"

// Export the factory function to create an instance of the class
EXPORT Model * createModel() {
    return new Model_OGL();
}

Model_OGL::Model_OGL()
	: m_vao{ 0 }
	, m_vbo{ 0 }
{
}

void Model_OGL::SetVertices(const VertexArray & vertices)
{
	if (m_vao == 0) m_vao = BufferManager::GetVAO();
	m_vbo = BufferManager::GetVBO();
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * 8 * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	// Vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture Coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
	_vertices = vertices;
}

void Model_OGL::SetIndices(const TriangleArray& indices)
{
}

void Model_OGL::Draw()
{
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
	for (auto & m : _meshes)
	{
		Mesh_OGL * mesh = dynamic_cast<Mesh_OGL *>(m.get());
		const TriangleArray & triangles = mesh->GetTriangles();
		glDrawElements(GL_TRIANGLES,       // mode
			mesh->GetTriangles().size(),   // number of indices
			GL_UNSIGNED_INT,               // type of the indices
		    triangles.data()               // pointer to the indices
		);
		mesh->Draw();
	}
	glBindVertexArray(0);
}