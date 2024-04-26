#pragma once

#include <common/Model.h>
#include <glad/glad.h>

class Model_OGL : public Model
{
public:
    Model_OGL();

    void SetVertices(const VertexArray& vertices) override;
    void SetIndices(const TriangleArray& indices) override;
    void Draw() override;

    Venom::ErrorCode ReloadObjectFromEngine() override;

private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
    int m_indicesCount;
};

extern "C"
{
    EXPORT Model * createModel();
}