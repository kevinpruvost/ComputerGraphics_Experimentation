#include <common/Model.h>

#include <unordered_map>
#include <unordered_set>

std::vector<Mesh> meshes;
std::vector<Material> materials;

// Converts Assimp's aiMatrix4x4 to GLM's mat4
glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from) {
    glm::mat4 to;
    to[0][0] = from.a1; to[0][1] = from.b1; to[0][2] = from.c1; to[0][3] = from.d1;
    to[1][0] = from.a2; to[1][1] = from.b2; to[1][2] = from.c2; to[1][3] = from.d2;
    to[2][0] = from.a3; to[2][1] = from.b3; to[2][2] = from.c3; to[2][3] = from.d3;
    to[3][0] = from.a4; to[3][1] = from.b4; to[3][2] = from.c4; to[3][3] = from.d4;
    return to;
}

// Load a single mesh
Venom::ErrorCode Model::Assimp_ProcessMesh(aiMesh* mesh, const aiMatrix4x4& from) {
    if (!mesh->HasFaces()) {
        Logger::DebugPrint("Mesh [%s] does not have faces.", mesh->mName.C_Str());
    }

    if (!mesh->HasNormals()) {
        Logger::DebugPrint("Mesh [%s] does not have normals.", mesh->mName.C_Str());
    }

    if (!mesh->HasTextureCoords(0)) {
        Logger::DebugPrint("Mesh [%s] does not have texture coordinates.", mesh->mName.C_Str());
    }

    if (!mesh->HasTangentsAndBitangents()) {
        Logger::DebugPrint("Mesh [%s] does not have tangents and bitangents.", mesh->mName.C_Str());
    }

    Mesh * meshResource = Mesh::CreateMesh();
    std::string name = "Mesh_";
    name += GetResourceName();
    name += '_';
    name += mesh->mName.C_Str();
    meshResource->SetResourceName(name.c_str());
    Resources::AddResource(name.c_str(), meshResource);

    VertexArray vertices(mesh->mNumVertices);
    glm::mat4 transformMat = aiMatrix4x4ToGlm(from);

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        glm::vec4 pos(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.0f);
        pos = transformMat * pos;
        memcpy(&vertices[i].pos, &pos, sizeof(glm::vec3));

        if (mesh->HasNormals()) {
            glm::vec3 normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            normal = glm::mat3(transformMat) * normal;
            memcpy(&vertices[i].normals, &normal, sizeof(glm::vec3));
        }

        if (mesh->HasTextureCoords(0)) {
            vertices[i].textureCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }

        if (mesh->HasTangentsAndBitangents()) {
            glm::vec3 tangent(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
            tangent = glm::mat3(transformMat) * tangent;
            memcpy(&vertices[i].tangent, &tangent, sizeof(glm::vec3));

            glm::vec3 bitangent(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
            bitangent = glm::mat3(transformMat) * bitangent;
            memcpy(&vertices[i].bitangent, &bitangent, sizeof(glm::vec3));
        }
    }

    VertexBuffer* vertexBuffer = VertexBuffer::CreateVertexBuffer();
    vertexBuffer->SetHasNormals(mesh->HasNormals());
    vertexBuffer->SetHasTextureCoords(mesh->HasTextureCoords(0));
    vertexBuffer->SetHasTangentsAndBitangents(mesh->HasTangentsAndBitangents());
    if (mesh->HasFaces())
    {
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
        vertexBuffer->SetVertices(vertices, indices);
    }
    else {
        vertexBuffer->SetVertices(vertices);
    }
    meshResource->SetVertexBuffer(vertexBuffer);
    meshResource->SetMaterialId(mesh->mMaterialIndex);
    _meshes.push_back(meshResource);
    return Venom::ErrorCode::Success;
}

// Recursively process each node in the scene graph
Venom::ErrorCode Model::Assimp_ProcessNode(aiNode* node, const aiScene* scene, const aiMatrix4x4& parentTransform) {
    // Process all the node's meshes
    aiMatrix4x4 nodeTransform = parentTransform * node->mTransformation;

    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        if (Assimp_ProcessMesh(mesh, nodeTransform) != Venom::ErrorCode::Success) {
            return Venom::ErrorCode::Failure;
        }
    }

    // Recursively process each of the node's children
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        if (Assimp_ProcessNode(node->mChildren[i], scene, nodeTransform) != Venom::ErrorCode::Success) {
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
        std::string name = "Material_";
        name += GetResourceName();
        name += '_';
        name += ainame.C_Str();

        Material * material = Resources::Create<Material>(name.c_str());
        if (material == nullptr) {
            Logger::Print("Error loading material in model parsing.");
            return Venom::ErrorCode::Failure;
        }

        // Checks every type of texture
#ifdef __DEBUG
        for (aiTextureType j = aiTextureType_DIFFUSE; j < aiTextureType_UNKNOWN; j = (aiTextureType)(j + 1)) {
            auto test1 = mat->GetTextureCount(j);
            Logger::Print("Texture count: %d for type: %d", test1, j);
        }
#endif

        aiString path;
        aiColor4D color;

        // Check for alpha properties
        float alphaValue = 1.0f;
        if (mat->Get(AI_MATKEY_OPACITY, alphaValue) == AI_SUCCESS && alphaValue != 1.0f) {
            Logger::Print("Has opacity value: %.5f!!", alphaValue);
        }

        // Check for alpha multiplier (custom property or assumption based on GLTF convention)
        aiColor4D baseColor;
        if (mat->Get(AI_MATKEY_BASE_COLOR, baseColor) == AI_SUCCESS) {
            material->SetBaseColor({baseColor.r, baseColor.g, baseColor.b, baseColor.a});
        }

        // Diffuse color
        glm::vec4 * glmColor = (glm::vec4 *)(&color);
        if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
            material->SetDiffuse(*glmColor);
        }
        // Diffuse Texture
        if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
                std::filesystem::path pathStr(path.C_Str());
                std::string textureName = pathStr.stem().string();
                Texture* texture = Resources::Load<Texture>(textureName.c_str());
                if (texture == nullptr) {
                    texture = Texture::CreateTexture();
                    texture->CreateFromFile(modelPath / pathStr.filename());
                    texture->SetResourceName(textureName.c_str());
                    Resources::AddResource(textureName.c_str(), texture);
                }
                material->SetDiffuse(texture);
                material->AddTexture(texture);
            }
        }

        // Ambient color
        if (mat->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS) {
            material->SetAmbient(*glmColor);
        }
        // Ambient Texture
        if (mat->GetTextureCount(aiTextureType_AMBIENT) > 0) {
            if (mat->GetTexture(aiTextureType_AMBIENT, 0, &path) == AI_SUCCESS) {
                std::filesystem::path pathStr(path.C_Str());
                std::string textureName = pathStr.stem().string();
                Texture* texture = Resources::Load<Texture>(textureName.c_str());
                if (texture == nullptr) {
                    texture = Texture::CreateTexture();
                    texture->CreateFromFile(modelPath / pathStr.filename());
                    texture->SetResourceName(textureName.c_str());
                    Resources::AddResource(textureName.c_str(), texture);
                }
                material->SetAmbient(texture);
            }
        }

        // Specular color
        if (mat->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS) {
            material->SetSpecular(*glmColor);
        }
        // Specular Texture
        if (mat->GetTextureCount(aiTextureType_SPECULAR) > 0) {
            if (mat->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS) {
                std::filesystem::path pathStr(path.C_Str());
                std::string textureName = pathStr.stem().string();
                Texture* texture = Resources::Load<Texture>(textureName.c_str());
                if (texture == nullptr) {
                    texture = Texture::CreateTexture();
                    texture->CreateFromFile(modelPath / pathStr.filename());
                    texture->SetResourceName(textureName.c_str());
                    Resources::AddResource(textureName.c_str(), texture);
                }
                material->SetSpecular(texture);
            }
        }

        // Normal Texture
        if (mat->GetTextureCount(aiTextureType_NORMALS) > 0) {
            if (mat->GetTexture(aiTextureType_NORMALS, 0, &path) == AI_SUCCESS) {
                std::filesystem::path pathStr(path.C_Str());
                std::string textureName = pathStr.stem().string();
                Texture* texture = Resources::Load<Texture>(textureName.c_str());
                if (texture == nullptr) {
                    texture = Texture::CreateTexture();
                    texture->CreateFromFile(modelPath / pathStr.filename());
                    texture->SetResourceName(textureName.c_str());
                    Resources::AddResource(textureName.c_str(), texture);
                }
                material->SetNormal(texture);
            }
        }

        // Shininess
        float shininess;
        if (mat->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS) {
            material->SetShininess(shininess);
        }

        if (material->IsUsingAmbientTexture() || material->IsUsingDiffuseTexture() || material->IsUsingSpecularTexture() || material->IsUsingNormalTexture()) {
            UsePBR(true);
        }

        _materials.push_back(material);
    }
    return Venom::ErrorCode::Success;
}

Venom::ErrorCode Model::ParseModel(const std::filesystem::path& path)
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
    // Start processing from the root node with an identity matrix as the initial parent transform
    aiMatrix4x4 identity;
    Assimp_LoadMaterials(path.parent_path(), scene);
    Assimp_ProcessNode(scene->mRootNode, scene, identity);
    importer.FreeScene();
    return Venom::ErrorCode::Success;
}
