#include "Mesh.h"

#include "BufferManager.h"

// Export the factory function to create an instance of the class
EXPORT Mesh * createMesh() {
    return new Mesh_OGL();
}

void Mesh_OGL::SetVertices(const VertexArray & vertices)
{
	m_vao = BufferManager::GetVAO();
	m_vbo = BufferManager::GetVBO();
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * 8 * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
	m_vertices = vertices;
}

void Mesh_OGL::Draw()
{
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
    glBindVertexArray(0);
}
