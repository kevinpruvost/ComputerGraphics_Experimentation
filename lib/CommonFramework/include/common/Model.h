#pragma once

#include <common/DLL.h>
#include <common/Mesh.h>
#include <common/Material.h>
#include <common/Face.h>

class Model : public Drawable3D
{
public:
    virtual ~Model() = default;

    COMMONFRAMEWORK_API static Model * CreateModel();
    static inline Model* CreateModel(const std::filesystem::path& path)
    {
        Model * mesh = CreateModel();
        mesh->CreateFromFile(path);
        return mesh;
    }

    COMMONFRAMEWORK_API void CreateFromFile(const std::filesystem::path & path);

    virtual void SetVertices(const VertexArray& vertices) = 0;
    COMMONFRAMEWORK_API VertexArray GetVertices() const;
    virtual void SetIndices(const TriangleArray& indices) = 0;

    COMMONFRAMEWORK_API void AddMesh(Mesh * mesh);
    COMMONFRAMEWORK_API void AddMaterial(Material * material);

protected:
    Model() = default;

    std::vector<std::shared_ptr<Mesh>> _meshes;
    std::vector<std::shared_ptr<Material>> _materials;
    VertexArray _vertices;

private:
    // Parser
    void ParseObj(const std::filesystem::path & path);
};

