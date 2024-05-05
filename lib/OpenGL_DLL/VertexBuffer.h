#pragma once

#include <common/Vertex.h>
#include <glad/glad.h>

class Vertex_OGL : public VertexBuffer
{
public:
    Vertex_OGL();
    ~Vertex_OGL();

    void SetVertices(const VertexArray& vertices) override;
    void Bind() const override;
    void Unbind() const override;
    Venom::ErrorCode ReloadObjectFromEngine() override;

private:
    GLuint __vbo;
    GLuint __vao;
};

extern "C"
{
    EXPORT VertexBuffer * createVertexBuffer();
}