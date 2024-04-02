#include <common/Model.h>
#include <common/FrameworkLoader.h>

#include <fstream>

typedef Model * (*CreateModelFn)();

Model* Model::CreateModel()
{
    CreateModelFn createMeshFn = FrameworkLoader::EngineDll->getFunction<CreateModelFn>("createModel");
    if (createMeshFn == nullptr)
        throw DLLException("Failed to load createMesh function from engine dll");
    Model* mesh = createMeshFn();
    if (mesh == nullptr)
        throw DLLException("Failed to create mesh");
    return mesh;
}

void Model::CreateFromFile(const std::filesystem::path& path)
{
    VertexArray vertices;

    if (!std::filesystem::exists(path))
        throw RuntimeException("File does not exist: {}", path.string().c_str());

    const auto extension = path.extension();
    if (extension == ".obj") {
        ParseObj(path);
    } else if (extension == ".fbx")
    {

    }
    else
    {
        throw RuntimeException("Unsupported file format: {0}", extension.string().c_str());
    }
}

VertexArray Model::GetVertices() const
{
    return _vertices;
}

void Model::AddMesh(Mesh* mesh)
{
    _meshes.push_back(std::shared_ptr<Mesh>(mesh));
}

void Model::AddMaterial(Material* material)
{
    _materials.push_back(std::shared_ptr<Material>(material));
}
