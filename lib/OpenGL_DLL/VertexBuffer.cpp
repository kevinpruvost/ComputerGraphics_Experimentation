#include "VertexBuffer.h"

#include "BufferManager.h"

// Export the factory function to create an instance of the class
EXPORT VertexBuffer * createVertexBuffer() {
    return new Vertex_OGL();
}

Vertex_OGL::Vertex_OGL()
	: __vao{ 0 }
	, __vbo{ 0 }
{
}

Vertex_OGL::~Vertex_OGL()
{
}

void Vertex_OGL::SetVertices(const VertexArray& vertices)
{
	if (__vao == 0) __vao = BufferManager::GetVAO();
	if (__vbo == 0) __vbo = BufferManager::GetVBO();
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(__vao);

	glBindBuffer(GL_ARRAY_BUFFER, __vbo);
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

	_v = vertices;
}

void Vertex_OGL::Bind() const
{
	glBindVertexArray(__vao);
}

void Vertex_OGL::Unbind() const
{
	glBindVertexArray(0);
}

Venom::ErrorCode Vertex_OGL::ReloadObjectFromEngine()
{
	SetVertices(_v);
    return Venom::ErrorCode::Success;
}
