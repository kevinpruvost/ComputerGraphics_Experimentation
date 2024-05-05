#pragma once

#include <common/DLL.h>
#include <common/Mesh.h>
#include <common/Material.h>
#include <common/Face.h>
#include <common/ObjectPool.h>

class COMMONFRAMEWORK_API Model : public Drawable3D
{
public:
    virtual ~Model() = default;

    static Model * CreateModel();
    static inline Model* CreateModel(const std::filesystem::path& path)
    {
        Model * mesh = CreateModel();
        mesh->CreateFromFile(path);
        return mesh;
    }
    static inline Model* CreateSphereModel(float radius, int sectors, int stacks)
    {
        Model* mesh = CreateModel();
        mesh->CreateSphere(radius, sectors, stacks);
        return mesh;
    }
    static inline Model* CreateSquareModel()
    {
        Model* mesh = CreateModel();
        mesh->CreateSquare();
        return mesh;
    }

    Venom::ErrorCode CreateFromFile(const std::filesystem::path & path);
    Venom::ErrorCode CreateSphere(float radius, int sectors, int stacks);
    Venom::ErrorCode CreateSquare();

    void SetVertices(const VertexArray& vertices);
    void SetVertices(VertexBuffer * vertices);
    VertexArray GetVertices() const;
    virtual void SetIndices(const TriangleArray& indices) = 0;

    void AddMesh(Mesh * mesh);
    void AddMaterial(Material * material);

protected:
    Model();

    std::vector<std::shared_ptr<Mesh>> _meshes;
    std::vector<std::shared_ptr<Material>> _materials;
    VertexBuffer * _vertexBuffer;

private:
    // Parser
    void ParseObj(const std::filesystem::path & path);
};

