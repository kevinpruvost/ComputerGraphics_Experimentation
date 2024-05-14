#include <common/Model.h>
#include <common/Engine/EngineLoader.h>

#include <fstream>

typedef Model * (*CreateModelFn)();

Model* Model::Create()
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
        ParseFbx(path);
    } else if (extension == ".fbx")
    {
        ParseFbx(path);
    }
    else
    {
        Logger::Print("Unsupported file format: %s", extension.string().c_str());
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

    Mesh* meshResource = Mesh::CreateMesh();
    std::string name = fmt::format("{}_Sphere_Mesh", GetResourceName());
    meshResource->SetResourceName(name.c_str());
    Resources::AddResource(name.c_str(), meshResource);
    VertexBuffer* vertexBuffer = VertexBuffer::CreateVertexBuffer();
    vertexBuffer->SetHasNormals(true);
    vertexBuffer->SetHasTextureCoords(true);
    vertexBuffer->SetVertices(verticesProcessed);
    meshResource->SetVertexBuffer(vertexBuffer);
    _meshes.push_back(meshResource);
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode Model::CreateSquare()
{
    // Assembling the square vertices
    const VertexArray vertices = {
        { glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
        { glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f) },
        { glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) }
    };
    const VertexArray verticesProcessed = {
        vertices[0], vertices[1], vertices[2],
        vertices[0], vertices[2], vertices[3]
    };

    Mesh * meshResource = Mesh::CreateMesh();
    std::string name = fmt::format("{}_Square_Mesh", GetResourceName());
    meshResource->SetResourceName(name.c_str());
    Resources::AddResource(name.c_str(), meshResource);
    VertexBuffer* vertexBuffer = VertexBuffer::CreateVertexBuffer();
    vertexBuffer->SetHasNormals(true);
    vertexBuffer->SetHasTextureCoords(true);
    vertexBuffer->SetVertices(verticesProcessed);
    meshResource->SetVertexBuffer(vertexBuffer);
    _meshes.push_back(meshResource);
    return Venom::ErrorCode::Success;
}

void Model::AddMesh(Mesh* mesh)
{
    _meshes.push_back(Ptr<Mesh>(mesh));
}

void Model::AddMaterial(Material* material)
{
    _materials.push_back(Ptr<Material>(material));
}

void Model::SetShader(ShaderPipeline* shader)
{
    _shader = shader;
}

void Model::SetWireframeShader(ShaderPipeline* shader)
{
    _wireframeShader = shader;
}

Model::Model()
    : Drawable3D()
    , Resource(ResourceType::MODEL)
    , _shader{ nullptr }
    , _wireframeShader{ nullptr }
{
}

template<>
Model * Resources::_Load(const char const* name, const YamlNode& data)
{
    Logger::DebugPrint("Loading model: %s", name);

    Model* model = Model::Create();
    model->SetResourceName(name);
    std::string path = YamlNodeToString(data["path"]);
    if (path == "Square")
        model->CreateSquare();
    else if (path == "Sphere")
        model->CreateSphere(YamlNodeToFloat(data["radius"]), YamlNodeToInt(data["sectors"]), YamlNodeToInt(data["stacks"]));
    else
        model->CreateFromFile(path);
    return model;
}