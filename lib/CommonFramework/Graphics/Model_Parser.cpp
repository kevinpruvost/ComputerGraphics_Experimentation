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
                    vertex.normals = vertices[nIndex];
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
                    vertex.normals = vertices[nIndex];
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
    SetVertices(verticesProcessed);
}