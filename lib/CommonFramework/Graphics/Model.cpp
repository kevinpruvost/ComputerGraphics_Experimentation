#include <common/Model.h>
#include <common/Engine/EngineLoader.h>

#include <fstream>

typedef Model * (*CreateModelFn)();

Model* Model::CreateModel()
{
    CreateModelFn createMeshFn = EngineLoader::GetEngineDll()->getFunction<CreateModelFn>("createModel");
    assert(createMeshFn != nullptr);
    Model* model = createMeshFn();
    assert(model != nullptr);
    return model;
}

Venom::ErrorCode Model::CreateFromFile(const std::filesystem::path& path)
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
        Logger::Print("Unsupported file format: {0}", extension.string().c_str());
        return Venom::ErrorCode::Failure;
    }
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode Model::CreateSphere(float radius, int sectors, int stacks)
{
    // Assembling the sphere vertices
    VertexArray vertices;
    VertexArray verticesProcessed;

    const float stackStep = glm::pi<float>() / stacks;
    const float sectorStep = 2 * glm::pi<float>() / sectors;
    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = glm::pi<float>() / 2 - i * stackStep;
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * sectorStep;

            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            Vertex vertex;
            vertex.pos = glm::vec3(x, y, z);
            vertex.normals = glm::vec3(1.0f);
            vertex.textureCoords = glm::vec2((float)j / sectors, (float)i / stacks);

            vertices.push_back(vertex);
        }
    }

    // Form triangles
    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < sectors; ++j) {
            int nextRow = (i + 1) % (stacks + 1);
            int nextColumn = (j + 1) % (sectors + 1);

            // Forming two triangles for each quad
            verticesProcessed.push_back(vertices[i * (sectors + 1) + j]);
            verticesProcessed.push_back(vertices[i * (sectors + 1) + nextColumn]);
            verticesProcessed.push_back(vertices[nextRow * (sectors + 1) + j]);

            verticesProcessed.push_back(vertices[nextRow * (sectors + 1) + j]);
            verticesProcessed.push_back(vertices[i * (sectors + 1) + nextColumn]);
            verticesProcessed.push_back(vertices[nextRow * (sectors + 1) + nextColumn]);
        }
    }

    SetVertices(verticesProcessed);
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode Model::CreateSquare()
{
    // Assembling the square vertices
    const VertexArray verticesProcessed = {
        { glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
        { glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f) },
        { glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) }
    };
    SetVertices(verticesProcessed);
    return Venom::ErrorCode::Success;
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