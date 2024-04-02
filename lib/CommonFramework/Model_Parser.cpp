#include <common/Model.h>

#include <unordered_map>
#include <unordered_set>

void Model::ParseObj(const std::filesystem::path& path)
{
    std::ifstream file(path);

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textures;

    struct FullVertex
    {
        int v[3];
        bool operator==(const FullVertex& other) const { return v[0] == other.v[0] && v[1] == other.v[1] && v[2] == other.v[2]; }
        struct Hash {
            std::size_t operator()(const FullVertex& vertex) const { return std::hash<int>()(vertex.v[0]) ^ std::hash<int>()(vertex.v[1]) ^ std::hash<int>()(vertex.v[2]); }
        };
    };
    std::unordered_map<FullVertex, int, FullVertex::Hash> fullVertices;
    unsigned int fullVertexIndex = 0;
    TriangleArray facesToProcess;

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

                FullVertex vertex = { -1, -1, -1 };

                // v/t/n
                if (tokenStream >> vIndex >> delimiter >> tIndex >> delimiter >> nIndex)
                {
                    --vIndex; --nIndex; --tIndex;
                    vertex = { vIndex, nIndex, tIndex };
                    goto addFullVertex;
                }
                tokenStream.clear();
                tokenStream.seekg(0);
                // v/t
                if (tokenStream >> vIndex >> delimiter >> tIndex)
                {
                    --vIndex; --tIndex;
                    vertex = { vIndex, -1, tIndex };
                    goto addFullVertex;
                }
                tokenStream.clear();
                tokenStream.seekg(0);
                // v//n
                if (tokenStream >> vIndex >> delimiter >> delimiter >> nIndex)
                {
                    --vIndex; --nIndex;
                    vertex = { vIndex, nIndex, -1 };
                    goto addFullVertex;
                }
                tokenStream.clear();
                tokenStream.seekg(0);
                // v
                if (tokenStream >> vIndex)
                {
                    --vIndex;
                    vertex = { vIndex, -1, -1 };
                    goto addFullVertex;
                }
                addFullVertex:
                auto elem = fullVertices.find(vertex);
                if (elem == fullVertices.end()) {
                    fullVertices[vertex] = fullVertexIndex;
                    face.v[i++] = fullVertexIndex++;
                }
                else {
                    face.v[i++] = elem->second;
                }
            }
            facesToProcess.push_back(face);
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

    // Assembling the vertices
    VertexArray verticesProcessed(fullVertices.size());
    for (const auto & vertex : fullVertices)
    {
        auto vIndex = vertex.first.v[0];
        auto nIndex = vertex.first.v[1];
        auto tIndex = vertex.first.v[2];
        if (vIndex >= vertices.size())
            throw RuntimeException("Invalid vertex index");
        if (nIndex != -1 && nIndex >= normals.size())
            throw RuntimeException("Invalid normal index");
        if (tIndex != -1 && tIndex >= textures.size())
            throw RuntimeException("Invalid texture coord index");
        verticesProcessed[vertex.second] = {
            vertices[vIndex],
            nIndex != -1 ? normals[nIndex] : glm::vec3(0.0f),
            tIndex != -1 ? textures[tIndex] : glm::vec3(0.0f)
        };
    }
    SetVertices(verticesProcessed);

    // Assembling the faces
    SetIndices(facesToProcess);
}