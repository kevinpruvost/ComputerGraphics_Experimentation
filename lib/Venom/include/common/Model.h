#pragma once

#include <common/DLL.h>
#include <common/Mesh.h>
#include <common/Material.h>
#include <common/Face.h>
#include <common/Resources.h>

#include <common/ShaderPipeline.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class VENOM_API Model : virtual public Drawable3D, public Resource
{
public:
    virtual ~Model() = default;

    static inline Model* CreateModel(const std::filesystem::path& path)
    {
        Model * mesh = Create();
        mesh->CreateFromFile(path);
        return mesh;
    }
    static inline Model* CreateSphereModel(float radius, int sectors, int stacks)
    {
        Model* mesh = Create();
        mesh->CreateSphere(radius, sectors, stacks);
        return mesh;
    }
    static inline Model* CreateSquareModel()
    {
        Model* mesh = Create();
        mesh->CreateSquare();
        return mesh;
    }

    Venom::ErrorCode CreateFromFile(const std::filesystem::path & path);
    Venom::ErrorCode CreateSphere(float radius, int sectors, int stacks);
    Venom::ErrorCode CreateSquare();

    virtual void SetIndices(const TriangleArray& indices) = 0;

    void AddMesh(Mesh * mesh);
    void AddMaterial(Material * material);

    void SetShader(ShaderPipeline * shader);
    void SetWireframeShader(ShaderPipeline * shader);
    ShaderPipeline * GetShader() const;
    ShaderPipeline * GetWireframeShader() const;
    const std::vector<Ptr<Mesh>> & GetMeshes() const;
    const std::vector<Ptr<Material>> & GetMaterials() const;

protected:
    friend class Resources;
    static Model* Create();
    Model();

    std::vector<Ptr<Mesh>> _meshes;
    std::vector<Ptr<Material>> _materials;
    ShaderPipeline * _shader, * _wireframeShader;

private:
    // Parser
    Venom::ErrorCode ParseFbx(const std::filesystem::path& path);
    Venom::ErrorCode Assimp_ProcessNode(aiNode* node, const aiScene* scene);
    Venom::ErrorCode Assimp_ProcessMesh(aiMesh* mesh);
    Venom::ErrorCode Assimp_LoadMaterials(const std::filesystem::path& modelPath, const aiScene* scene);
};

