#pragma once

#include <common/Mesh.h>
#include <glad/glad.h>

class Mesh_OGL : public Mesh
{
public:
    void SetVertices(const Vec3Array& vertices) override;
    void Draw() override;

private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
    int m_indicesCount;
    Vec3Array m_vertices;
};

extern "C"
{
    EXPORT Mesh * createMesh();
}