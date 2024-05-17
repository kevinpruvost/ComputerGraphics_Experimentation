#include "VertexBuffer.h"

#include "BufferManager.h"

#include <common/ShaderPipeline.h>

// Export the factory function to create an instance of the class
EXPORT VertexBuffer * createVertexBuffer() {
    return new Vertex_OGL();
}

Vertex_OGL::Vertex_OGL()
	: __vao{ 0 }
	, __vbo{ 0 }
	, __ebo{ 0 }
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
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	// Vertex
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	// Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(float)));

	// Texture Coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(float)));

	// Tangents
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(8 * sizeof(float)));

	// Bitangents
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(11 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	_v = vertices;
}

void Vertex_OGL::SetVertices(const VertexArray& vertices, const IndexArray& indices)
{
	if (__vao == 0) __vao = BufferManager::GetVAO();
	if (__vbo == 0) __vbo = BufferManager::GetVBO();
	if (__ebo == 0) __ebo = BufferManager::GetEBO();
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(__vao);

	glBindBuffer(GL_ARRAY_BUFFER, __vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, __ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	// Vertex
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	// Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(float)));

	// Texture Coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(float)));

	// Tangents
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(8 * sizeof(float)));

	// Bitangents
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(11 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	_v = vertices;
	_i = indices;
}

void Vertex_OGL::Bind() const
{
	glBindVertexArray(__vao);
}

void Vertex_OGL::Unbind() const
{
	glBindVertexArray(0);
}

void Vertex_OGL::Draw() const
{
	auto shader = ShaderPipeline::GetCurrentlyUsedPipeline();
	if (shader->HasTesselationStage()) {
		glDrawArrays(GL_PATCHES, 0, _v.size());
	} else {
		if (__ebo != 0)
			glDrawElements(GL_TRIANGLES, _i.size(), GL_UNSIGNED_INT, 0);
		else
			glDrawArrays(GL_TRIANGLES, 0, _v.size());
	}
}

Venom::ErrorCode Vertex_OGL::ReloadObjectFromEngine()
{
	SetVertices(_v);
    return Venom::ErrorCode::Success;
}
