#include <common/Model.h>

#include <unordered_map>
#include <unordered_set>

void Model::ParseObj(const std::filesystem::path& path)
{
    std::ifstream file(path);

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textures;

    // Assembling the vertices
    VertexArray verticesProcessed;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        } else if (prefix == "vn") {
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        } else if (prefix == "vt") {
            glm::vec2 texCoord;
            iss >> texCoord.s >> texCoord.t;
            textures.push_back(texCoord);
        } else if (prefix == "f") {
            TriangleFace face;
            char delimiter;
            int vIndex, nIndex, tIndex;
            int i = 0;
            std::vector<std::string> tokens;
            std::string token;

            // Separate indices (f v/t/n v/t/n v/t/n) or (f v//n v//n v//n) or (f v v v) ...
            while (std::getline(iss, token, ' ')) if (!token.empty()) tokens.push_back(token);

            if (tokens.size() > 3)
                throw RuntimeException("Only triangles for obj models are supported");

            for (auto & token: tokens) {
                std::istringstream tokenStream(token);

                Vertex vertex;

                // v/t/n
                if (tokenStream >> vIndex >> delimiter >> tIndex >> delimiter >> nIndex)
                {
                    --vIndex; --nIndex; --tIndex;
                    vertex.pos = vertices[vIndex];
                    vertex.normals = normals[nIndex];
                    vertex.textureCoords = textures[tIndex];
                    goto addFullVertex;
                }
                tokenStream.clear();
                tokenStream.seekg(0);
                // v/t
                if (tokenStream >> vIndex >> delimiter >> tIndex)
                {
                    --vIndex; --tIndex;
                    vertex.pos = vertices[vIndex];
                    vertex.textureCoords = vertices[tIndex];
                    goto addFullVertex;
                }
                tokenStream.clear();
                tokenStream.seekg(0);
                // v//n
                if (tokenStream >> vIndex >> delimiter >> delimiter >> nIndex)
                {
                    --vIndex; --nIndex;
                    vertex.pos = vertices[vIndex];
                    vertex.normals = normals[nIndex];
                    goto addFullVertex;
                }
                tokenStream.clear();
                tokenStream.seekg(0);
                // v
                if (tokenStream >> vIndex)
                {
                    --vIndex;
                    vertex.pos = vertices[vIndex];
                    goto addFullVertex;
                }
                addFullVertex:
                verticesProcessed.push_back(vertex);
            }
        } else if (prefix == "s") {
            // Ignore smooth group
        } else if (prefix == "mtllib") {
            // Material library
        } else if (prefix == "usemtl") {
            // Use material
        } else if (prefix == "g") {
            // Mesh name
        } else if (prefix == "o") {
            // Object name
        } else {
            throw RuntimeException("Unsupported prefix: {0}", prefix);
        }
    }
    Mesh* meshResource = Mesh::CreateMesh();
    std::string name = path.stem().string() + "_mesh";
    meshResource->SetResourceName(name.c_str());
    Resources::AddResource(name.c_str(), meshResource);
    VertexBuffer* vertexBuffer = VertexBuffer::CreateVertexBuffer();
    vertexBuffer->SetVertices(verticesProcessed);
    meshResource->SetVertexBuffer(vertexBuffer);
    _meshes.push_back(meshResource);
}

std::vector<Mesh> meshes;
std::vector<Material> materials;

// Load a single mesh
Venom::ErrorCode Model::Assimp_ProcessMesh(aiMesh* mesh) {
    if (!mesh->HasFaces()) {
        Logger::Print("Mesh does not have faces.");
        return Venom::ErrorCode::Failure;
    }

    if (!mesh->HasNormals()) {
        Logger::Print("Mesh [%s] does not have normals.", mesh->mName.C_Str());
        return Venom::ErrorCode::Failure;
    }

    if (!mesh->HasTextureCoords(0)) {
        Logger::Print("Mesh [%s] does not have texture coordinates.", mesh->mName.C_Str());
    }

    Mesh * meshResource = Mesh::CreateMesh();
    std::string name = GetResourceName();
    name += "_";
    name += mesh->mName.C_Str();
    meshResource->SetResourceName(name.c_str());
    Resources::AddResource(name.c_str(), meshResource);

    VertexArray vertices(mesh->mNumVertices);
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        memcpy(&vertices[i].pos, &mesh->mVertices[i], sizeof(glm::vec3));
        memcpy(&vertices[i].normals, &mesh->mNormals[i], sizeof(glm::vec3));
        if (mesh->HasTextureCoords(0))
            memcpy(&vertices[i].textureCoords, &mesh->mTextureCoords[0][i], sizeof(glm::vec2));
        //vertices[i].pos = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        //vertices[i].normals = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        //vertices[i].textureCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
    }
    IndexArray indices(mesh->mNumFaces * 3);
    int index = 0;
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        if (face.mNumIndices != 3) {
            Logger::Print("Face is not a triangle.");
            return Venom::ErrorCode::Failure;
        }
        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
            indices[index++] = face.mIndices[j];
        }
    }
    VertexBuffer * vertexBuffer = VertexBuffer::CreateVertexBuffer();
    vertexBuffer->SetVertices(vertices, indices);
    meshResource->SetVertexBuffer(vertexBuffer);
    meshResource->SetMaterialId(mesh->mMaterialIndex);
    _meshes.push_back(meshResource);
    return Venom::ErrorCode::Success;
}

// Recursively process each node in the scene graph
Venom::ErrorCode Model::Assimp_ProcessNode(aiNode* node, const aiScene* scene) {
    // Process all the node's meshes
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        if (Assimp_ProcessMesh(mesh) != Venom::ErrorCode::Success) {
            return Venom::ErrorCode::Failure;
        }
    }

    // Recursively process each of the node's children
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        if (Assimp_ProcessNode(node->mChildren[i], scene) != Venom::ErrorCode::Success) {
            return Venom::ErrorCode::Failure;
        }
    }
    return Venom::ErrorCode::Success;
}

// Load materials
Venom::ErrorCode Model::Assimp_LoadMaterials(const std::filesystem::path& modelPath, const aiScene* scene) {
    for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
        aiMaterial* mat = scene->mMaterials[i];

        // Get the name of the material
        aiString ainame;
        mat->Get(AI_MATKEY_NAME, ainame);
        std::string name = GetResourceName();
        name += '_';
        name += ainame.C_Str();

        Material * material = Resources::Create<Material>(name.c_str());
        if (material == nullptr) {
            Logger::Print("Error loading material in model parsing.");
            return Venom::ErrorCode::Failure;
        }

        // Diffuse color
        aiColor4D color;
        glm::vec4 * glmColor = (glm::vec4 *)(&color);
        if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
            material->SetDiffuse(*glmColor);
        }

        // Ambient color
        if (mat->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS) {
            material->SetAmbient(*glmColor);
        }

        // Specular color
        if (mat->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS) {
            material->SetSpecular(*glmColor);
        }

        // Shininess
        float shininess;
        if (mat->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS) {
            material->SetShininess(shininess);
        }

        // Load textures
        for (unsigned int j = 0; j < mat->GetTextureCount(aiTextureType_DIFFUSE); ++j) {
            aiString path;
            if (mat->GetTexture(aiTextureType_DIFFUSE, j, &path) == AI_SUCCESS) {
                std::filesystem::path pathStr(path.C_Str());
                std::string name = pathStr.stem().string();
                Texture * texture = Texture::CreateTexture();
                texture->CreateFromFile(modelPath / pathStr.filename());

                texture->SetResourceName(name.c_str());
                Resources::AddResource(name.c_str(), texture);
                material->AddTexture(texture);
            }
        }
        _materials.push_back(material);
    }
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode Model::ParseFbx(const std::filesystem::path& path)
{
    // Create an instance of the importer
    Assimp::Importer importer;

    // Specify post-processing options (e.g., triangulation)
    const aiScene* scene = importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    // Check if the scene was loaded successfully
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        // Handle error
        std::cerr << "Failed to load model: " << importer.GetErrorString() << std::endl;
        return Venom::ErrorCode::Failure;
    }

    // Process the meshes in the scene
    Assimp_LoadMaterials(path.parent_path(), scene);
    Assimp_ProcessNode(scene->mRootNode, scene);
    importer.FreeScene();
    return Venom::ErrorCode::Success;
}
